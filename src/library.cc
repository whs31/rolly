#include <rolly/library.h>

#include <array>
#include <atomic>
#include <algorithm>
#include <mutex>
#include <rolly/global/platform_definitions.h>
#include <rolly/string_util.h>
#include <oslayer/dlfcn.h>

using namespace std::literals;

namespace rolly {
  namespace detail {
    [[nodiscard]] std::string complete_suffix(std::filesystem::path const& name) {
      auto const str = name.generic_string();
      return {std::find(str.cbegin(), str.cend(), '.'), str.cend()};
    }
  }  // namespace detail

  struct library::library_private {
    library_private(std::filesystem::path path, library::load_hint const hints)
      : path(std::move(path))
      , hints(hints) {}

    library_private(library_private const&) = delete;
    library_private(library_private&&) = delete;
    library_private& operator=(library_private const&) = delete;
    library_private& operator=(library_private&&) = delete;

    ~library_private() { std::ignore = this->unload(); }

    [[nodiscard]] result<function_pointer_type> resolve(std::string_view const symbol) const {
      if(not this->handle.load(std::memory_order_relaxed))
        return error("library is not loaded");
      auto* symbol_addr =
        dlsym(this->handle.load(std::memory_order_relaxed), std::string(symbol).c_str());
      if(not symbol_addr)
        return error("symbol {:?} not found", symbol);
      return ok(
        reinterpret_cast<function_pointer_type>(symbol_addr)  // NOLINT(*-pro-type-reinterpret-cast)
      );
    }

    [[nodiscard]] result<> load() noexcept {
      if(this->handle.load(std::memory_order_relaxed))
        return error("library is already loaded");
      if(this->path.empty())
        return error("library path is empty");
      auto dl_flags = 0;
      if(static_cast<bool>(
           this->hints & library::load_hint::resolve_all_symbols
         ))  // NOLINT(*-branch-clone)
        dl_flags |= RTLD_NOW;
      else
        dl_flags |= RTLD_LAZY;
      if(static_cast<bool>(this->hints & library::load_hint::export_external_symbols))
        dl_flags |= RTLD_GLOBAL;
      else
        dl_flags |= RTLD_LOCAL;
#if defined(RTLD_DEEPBIND)
      if(static_cast<bool>(this->hints & library::load_hint::deep_bind))
        dl_flags |= RTLD_DEEPBIND;
#endif
      auto* h = dlopen(this->path.string().c_str(), dl_flags);
      if(not h)
        return error("failed to load library: {}", dlerror());
      this->handle.store(h, std::memory_order_relaxed);
      return ok();
    }

    [[nodiscard]] result<> unload() noexcept {
      auto* h = this->handle.load(std::memory_order_relaxed);
      if(h == nullptr)
        return error("library was already unloaded");
      auto const ret_code = dlclose(h);
      this->handle.store(nullptr, std::memory_order_relaxed);
      if(ret_code != 0)
        return error("failed to unload library with code {}: {}", ret_code, dlerror());
      return ok();
    }

    std::filesystem::path path;
    library::load_hint hints;
    std::atomic<void*> handle {nullptr};
  };

  library::library(std::filesystem::path path, library::load_hint hints)
    : m_d(std::make_unique<library_private>(std::move(path), hints)) {}

  library::library(library&&) = default;
  library& library::operator=(library&&) = default;
  library::~library() = default;

  std::filesystem::path const& library::path() const { return d().path; }

  std::string library::filename() const { return d().path.filename().string(); }

  library::load_hint library::load_hints() const { return d().hints; }

  bool library::loaded() const { return d().handle.load(std::memory_order_relaxed); }

  result<> library::load() noexcept {
    if(not this->m_d)
      return error("library d_ptr is null");
    return d().load();
  }

  result<> library::unload() noexcept {
    if(not this->m_d)
      return error("library d_ptr is null");
    return d().unload();
  }

  result<library::function_pointer_type> library::resolve(std::string_view const symbol) noexcept {
    if(not this->m_d)
      return error("library d_ptr is null");
    return d().resolve(symbol);
  }

  bool library::is_library(std::filesystem::path const& path) {
    auto const name = path.filename();
#if defined(ROLLY_OS_WINDOWS)
    return name.extension() == ".dll" or name.extension() == ".DLL";
#else
    auto const complete_ext = detail::complete_suffix(name);
    if(complete_ext.empty())
      return false;
    auto const suffixes = split_by(complete_ext, '.');
    auto constexpr valid_suffix_list = std::array {
#  if defined(ROLLY_OS_DARWIN) || defined(ROLLY_OS_IOS)
      "dylib"sv,
      "so"sv,
      "bundle"sv
#  elif defined(ROLLY_OS_LINUX) || defined(ROLLY_OS_FREEBSD) || defined(ROLLY_OS_ANDROID)
      "so"sv
#  else
#    error "Unsupported OS"
#  endif
    };
    auto const suffix = std::find(suffixes.cbegin(), suffixes.cend(), valid_suffix_list);
    return suffix != suffixes.cend();
#endif
  }
}  // namespace rolly
