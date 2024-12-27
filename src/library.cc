#include <rolly/library.h>

#include <array>
#include <atomic>
#include <algorithm>
#include <mutex>
#include <rolly/global/platform_definitions.h>
#include <rolly/string_util.h>
#include <oslayer/library.h>

using namespace std::literals;

namespace rolly {
  namespace detail {
    [[nodiscard]] std::string complete_suffix(std::filesystem::path const& name) {
      return {std::find(name.begin(), name.end(), '.'), name.end()};
    }
  }  // namespace detail

  struct library::library_private {
    [[nodiscard]] function_pointer_type resolve(std::string_view const symbol) {
      if(not this->handle.load(std::memory_order_relaxed))
        return nullptr;
      return reinterpret_cast<function_pointer_type>(
        oslayer::lib_resolve(this->handle, symbol)  // NOLINT(*-pro-type-reinterpret-cast)
      );
    }

    [[nodiscard]] result<> load() noexcept {
      if(this->handle.load(std::memory_order_relaxed)) {
        ++this->unload_count;
        return ok();
      }
      if(this->path.empty())
        return error("library path is empty");
      bool ret = oslayer::lib_load(
      );  // https://codebrowser.dev/qt5/qtbase/src/corelib/plugin/qlibrary_unix.cpp.html#_ZN15QLibraryPrivate8load_sysEv
      if(not ret)
        return error(oslayer::lib_last_error());
      ++this->ref_count;
      ++this->unload_count;
      return ok();
    }

    std::filesystem::path path;
    bool did_load {false};
    std::atomic<void*> handle {nullptr};
    std::mutex mutex;
    std::atomic<isize> ref_count {0};
    std::atomic<isize> unload_count {0};
  };

  library::library()
    : m_d(std::make_unique<library_private>()) {}

  library::library(std::filesystem::path const& path)
    : m_d(std::make_unique<library_private>()) {
    this->set_path(path);
  }

  library::~library() {}

  std::filesystem::path const& library::path() const {}

  std::string library::filename() const {}

  void library::set_path(std::filesystem::path const& path) {}

  library::load_hint library::load_hints() const {}

  void library::set_load_hints(load_hint hint) {}

  bool library::loaded() const { return this->m_d and d().handle.load(std::memory_order_relaxed); }

  result<> library::load() noexcept {
    if(not this->m_d)
      return error("library already unloaded");
    if(d().did_load) {
      auto const* h = d().handle.load(std::memory_order_relaxed);
      return h != nullptr ? ok() : error("failed to load library");
    }
    d().did_load = true;
    return d().load();
  }

  result<> library::unload() noexcept {}

  result<library::function_pointer_type> library::resolve(std::string_view const symbol) noexcept {
    if(not this->loaded())
      if(auto const res = this->load(); not res)
        return error(res.error());
    if(auto* p = d().resolve(symbol))
      return ok(p);
    return error("symbol {:?} not found", symbol);
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

  result<library::function_pointer_type>
    library::resolve(std::filesystem::path const& path, std::string_view const symbol) noexcept {
    return library(path)[symbol];
  }
}  // namespace rolly
