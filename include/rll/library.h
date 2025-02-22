#pragma once

#include <string>
#include <string_view>
#include "global/export.h"
#include "traits/pimpl.h"
#include "types/result.h"
#include "utility.h"
#ifndef Q_MOC_RUN
#  include <filesystem>
#endif

namespace rolly {
  class RLL_API library final {
   public:
    /**
     * @brief Dummy function pointer type, meant for casting to real function pointers.
     */
    using function_pointer_type = void (*)();
    /**
     * @brief Library load hints.
     * This enum describes the possible hints that can be used to change the way libraries are
     * handled when they are loaded.
     *
     * These values indicate how symbols are resolved when libraries
     * are loaded, and are specified using the @ref set_load_hints method.
     */
    enum class load_hint {
      /**
       * @brief No hints are set.
       */
      none = 0x00,

      /**
       * @brief Causes all symbols in a library to be resolved when it is loaded, not simply when
       * @ref resolve is called.
       */
      resolve_all_symbols = 0x01,

      /**
       * @brief Exports unresolved and external symbols in the library so that they can be resolved
       * in other dynamically-loaded libraries loaded later.
       */
      export_external_symbols = 0x02,

      /**
       * @brief Allows the file name of the library to specify a particular object file within an
       * archive file.
       * If this hint is given, the filename of the library consists of a path, which
       * is a reference to an archive file, followed by a reference to the archive member.
       */
      load_archive_member = 0x04,

      /**
       * @brief Prevents the library from being unloaded from the address space if @ref close is
       * called. The library's static variables are not reinitialized if @ref open is called at a
       * later time.
       */
      prevent_unload = 0x08,

      /**
       * @brief Instructs the linker to prefer definitions in the loaded library over exported
       * definitions in the loading application when resolving external symbols in the loaded
       * library. This option is only supported on Linux.
       */
      deep_bind = 0x10
    };

    /**
     * @brief Constructs a library object that will load the library specified
     * by path.
     * @remark I recommend omitting the file's suffix in fileName, since library will automatically
     * look for the file with the appropriate suffix in accordance with the platform, e.g. __".so"__
     * on Unix, __".dylib"__ on macOS and iOS, and __".dll"__ on Windows. (See path() and
     * filename() for more information.)
     * @param path Filename or absolute path to the library to load.
     * @param hints Load hints
     */
    explicit library(
      std::filesystem::path path,
      library::load_hint hints = library::load_hint::none
    );

    library(library const&) = delete;
    library& operator=(library const&) = delete;
    library(library&&);
    library& operator=(library&&);

    /**
     * @brief Destroys the library object and unloads the library.
     */
    ~library();

    /**
     * @brief Returns the file name or an absolute path to the library.
     * @details When loading the library, library searches in all system-specific library
     * locations (for example, `LD_LIBRARY_PATH` on Unix), unless the file name has an
     * absolute path. After loading the library successfully, path returns the fully-qualified
     * file name of the library, including the full path to the library if one was given in
     * the constructor or passed to @ref set_path.
     *
     * For example, after successfully loading the "GL" library on Unix platforms,
     * `path()` will return "libGL.so". If the file name was originally passed as
     * "/usr/lib/libGL", `path()` will return "/usr/lib/libGL.so".
     * @remark I recommend omitting the file's suffix in the file name, since library will
     * automatically look for the file with the appropriate suffix (see @ref is_library for more
     * info).
     * @return The file name or an absolute path to the library.
     */
    [[nodiscard]] std::filesystem::path const& path() const;

    /**
     * @brief Returns the file name part of the library's path.
     * @return The file name part of the library's path.
     */
    [[nodiscard]] std::string filename() const;

    /**
     * @brief Give the @ref load function some hints on how it should behave.
     * @details You can give some hints on how the symbols are resolved.
     * Usually, the symbols are not resolved at load time, but resolved lazily,
     * (that is, when @ref resolve is called). If you set the load hints to
     * load_hints::resolve_all_symbols, then all symbols will be resolved at load time
     * if the platform supports it.
     *
     * Setting load_hints::export_external_symbols will make the external symbols in the
     * library available for resolution in subsequent loaded libraries.
     *
     * If load_hints::load_archive_member is set, the file name is composed of two
     * components: A path which is a reference to an archive file followed by the second
     * component which is the reference to the archive member.
     * For instance, the fileName libGL.a(`shr_64.o`) will refer to the library `shr_64.o`
     * in the archive file named libGL.a. This is only supported on the AIX platform.
     *
     * The interpretation of the load hints is platform dependent, and if you use it you are
     * probably making some assumptions on which platform you are compiling for, so use them
     * only if you understand the consequences of them.
     *
     * By default, none of these flags are set, so libraries will be loaded with lazy symbol
     * resolution, and will not export external symbols for resolution in other
     * dynamically-loaded libraries.
     * @note Hints can only be cleared when this object is not associated with a file. Hints can
     * only be added once the file name is set (hints will be or'ed with the old hints).
     *
     * @note Setting this property after the library has been loaded has no effect and load_hints
     * will not reflect those changes.
     *
     * @note This property is shared among all library instances that refer to the same library.
     * @return The load hints
     */
    [[nodiscard]] load_hint load_hints() const;

    /**
     * @brief Returns `true` if the library is loaded; otherwise returns `false`.
     * @return `true` if the library is loaded; otherwise `false`.
     */
    [[nodiscard]] bool loaded() const;

    /**
     * @copydoc loaded
     */
    [[nodiscard]] explicit inline operator bool() const noexcept { return this->loaded(); }

    /**
     * @brief Loads the library.
     * @details Since resolve() always calls this function before resolving any symbols it is not
     * necessary to call it explicitly. In some situations you might want the library loaded in
     * advance, in which case you would use this function.
     * @return Loading result
     * @see loaded
     * @see unload
     * @see resolve
     */
    [[nodiscard]] result<> load() noexcept;

    /**
     * @brief Unloads the library.
     * @details This happens automatically on application termination, so you shouldn't normally
     * need to call this function.
     *
     * If other instances of library are using the same library, the call will fail, and unloading
     * will only happen when every instance has called unload().
     *
     * @warning Note that on macOS, dynamic libraries cannot be unloaded. library::unload() will
     * return `ok`, but the library will remain loaded into the process.
     * @return Unloading result
     * @see load
     * @see resolve
     */
    [[nodiscard]] result<> unload() noexcept;

    /**
     * @brief Returns the address of the exported symbol.
     * @details The library is loaded if necessary.
     * The function returns error if the symbol could not be resolved or if the library could not be
     * loaded.
     *
     * Example:
     * ```cpp
     * using fn = int (*)(int, int);
     * auto add = lib.resolve("add")
     *   .map([](auto* f) { return reinterpret_cast<fn>(f)(1, 2); });
     * if(add)
     *   std::cout << add(1, 2) << std::endl; // => 3
     * else
     *   std::cout << "error" << std::endl;
     * ```
     *
     * The symbol must be exported as a C function from the library.
     * This means that the function must be wrapped in an `extern "C"` if the library is
     * compiled with a C++ compiler.
     * @note On Windows you must also explicitly export the function from the DLL using the
     * `__declspec(dllexport)` compiler directive.
     * @param symbol Name of the symbol
     * @return
     */
    [[nodiscard]] result<library::function_pointer_type> resolve(std::string_view symbol) noexcept;

    /**
     * @copydoc resolve
     */
    [[nodiscard]] result<library::function_pointer_type> inline operator[](
      std::string_view const symbol
    ) noexcept {
      return this->resolve(symbol);
    }

    /**
     * @brief Resolves the symbol (see resolve()) and casts the result to function type T.
     * @tparam F Function type
     * @param symbol Name of the exported symbol
     * @return The cast address of the exported symbol or an error string
     * @see resolve
     */
    template <typename F>
    [[nodiscard]] inline result<F> resolve_cast(std::string_view const symbol) noexcept {
      return this->resolve(symbol).map([](auto* f) { return reinterpret_cast<F>(f); });
    }

    /**
     * @brief Returns `true` if __path__ has a valid suffix for a loadable library; otherwise
     * returns `false`.
     * @details Supported suffixes:
     *
     * | Platform      | Valid suffixes             |
     * |---------------|----------------------------|
     * | Windows       | `.dll`, `.DLL`             |
     * | Unix/Linux    | `.so`                      |
     * | macos and iOS | `.dylib`, `.bundle`, `.so` |
     * @remark Trailing versioning numbers on Unix are ignored.
     * @param path Filename or absolute path to the library
     * @return `true` if __path__ has a valid suffix for a loadable library; otherwise `false`
     */
    [[nodiscard]] static bool is_library(std::filesystem::path const& path);

   private:
    DECLARE_PRIVATE_UNIQUE_PTR_AS(library_private);
  };

  /**
   * @brief Flag bitwise or operator for rolly::library::load_hint
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise or
   * @relates rolly::library
   */
  [[nodiscard]] constexpr rolly::library::load_hint
    operator|(rolly::library::load_hint const lhs, rolly::library::load_hint const rhs) noexcept {
    return static_cast<rolly::library::load_hint>(
      rolly::to_underlying(lhs) | rolly::to_underlying(rhs)
    );
  }

  /**
   * @brief Flag bitwise and operator for rolly::library::load_hint
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise and
   * @relates rolly::library
   */
  [[nodiscard]] constexpr rolly::library::load_hint
    operator&(rolly::library::load_hint const lhs, rolly::library::load_hint const rhs) noexcept {
    return static_cast<rolly::library::load_hint>(
      rolly::to_underlying(lhs) & rolly::to_underlying(rhs)
    );
  }

  /**
   * @brief Flag bitwise xor operator for rolly::library::load_hint
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise xor
   * @relates rolly::library
   */
  [[nodiscard]] constexpr rolly::library::load_hint
    operator^(rolly::library::load_hint const lhs, rolly::library::load_hint const rhs) noexcept {
    return static_cast<rolly::library::load_hint>(
      rolly::to_underlying(lhs) ^ rolly::to_underlying(rhs)
    );
  }

  /**
   * @brief Flag bitwise not operator for rolly::library::load_hint
   * @param value Operand
   * @return Result of bitwise not
   * @relates rolly::library
   */
  [[nodiscard]] constexpr rolly::library::load_hint operator~(rolly::library::load_hint const value
  ) noexcept {
    return static_cast<rolly::library::load_hint>(~rolly::to_underlying(value));
  }

  /**
   * @brief Assign bitwise or operator for rolly::library::load_hint
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise or
   * @relates rolly::library
   */
  [[nodiscard]] constexpr rolly::library::load_hint&
    operator|=(rolly::library::load_hint& lhs, rolly::library::load_hint const rhs) noexcept {
    lhs = lhs | rhs;
    return lhs;
  }

  /**
   * @brief Assign bitwise and operator for rolly::library::load_hint
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise and
   * @relates rolly::library
   */
  [[nodiscard]] constexpr rolly::library::load_hint&
    operator&=(rolly::library::load_hint& lhs, rolly::library::load_hint const rhs) noexcept {
    lhs = lhs & rhs;
    return lhs;
  }

  /**
   * @brief Assign bitwise xor operator for rolly::library::load_hint
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise xor
   * @relates rolly::library
   */
  [[nodiscard]] constexpr rolly::library::load_hint&
    operator^=(rolly::library::load_hint& lhs, rolly::library::load_hint const rhs) noexcept {
    lhs = lhs ^ rhs;
    return lhs;
  }
}  // namespace rolly
