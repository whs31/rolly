#include <rll/library.h>

#include <set>
#include <atomic>
#include <algorithm>
#include <mutex>
#include <rll/global/platform_definitions.h>
#include <rll/string_util.h>
#include <oslayer/dlfcn.h>

using namespace std::literals;

namespace rll {
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
    : impl(std::make_unique<library_private>(std::move(path), hints)) {}

  library::library(library&&) = default;
  library& library::operator=(library&&) = default;
  library::~library() = default;

  std::filesystem::path const& library::path() const { return impl->path; }

  std::string library::filename() const { return impl->path.filename().string(); }

  library::load_hint library::load_hints() const { return impl->hints; }

  bool library::loaded() const { return impl->handle.load(std::memory_order_relaxed); }

  result<> library::load() noexcept {
    if(not impl)
      return error("library d_ptr is null");
    return impl->load();
  }

  result<> library::unload() noexcept {
    if(not impl)
      return error("library d_ptr is null");
    return impl->unload();
  }

  result<library::function_pointer_type> library::resolve(std::string_view const symbol) noexcept {
    if(not impl)
      return error("library d_ptr is null");
    return impl->resolve(symbol);
  }

  bool library::is_library(std::filesystem::path const& path) {
    auto const name = path.filename();
#if defined(RLL_OS_WINDOWS)
    return name.extension() == ".dll" or name.extension() == ".DLL";
#else
    auto const complete_ext = detail::complete_suffix(name);
    if(complete_ext.empty())
      return false;
    auto const suffixes = split_by(complete_ext, '.');
    auto valid_suffix_set = std::set<std::string> {};
#  if defined(RLL_OS_DARWIN) || defined(RLL_OS_IOS)
    valid_suffix_set.insert("dylib"s);
    valid_suffix_set.insert("bundle"s);
    valid_suffix_set.insert("so"s);
#  elif defined(RLL_OS_LINUX) || defined(RLL_OS_FREEBSD) || defined(RLL_OS_ANDROID)
    valid_suffix_set.insert("so"s);
#  endif
    for(auto const& suffix : suffixes)
      if(valid_suffix_set.find(suffix) != valid_suffix_set.end())
        return true;
    return false;
#endif
  }
}  // namespace rll
