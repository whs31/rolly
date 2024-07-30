#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <fmt/format.h>

#if defined(_WIN32)
# if defined(FLOPPY_LIBRARY)
#   define FLOPPY_EXPORT __declspec(dllexport)
# elif defined(FLOPPY_STATIC_LIBRARY)
#   define FLOPPY_EXPORT
# else // defined(FLOPPY_LIBRARY)
#   define FLOPPY_EXPORT __declspec(dllimport)
# endif // defined(FLOPPY_LIBRARY)
#else // defined(_WIN32)
# define FLOPPY_EXPORT
#endif // defined(_WIN32)

#define _stringify$(x) #x // NOLINT(*-macro-usage, *-identifier-naming)
#define stringify$(x) _stringify$(x) // NOLINT(*-macro-usage)

/// \brief Main namespace for the library.
/// \note Use <tt>fl</tt> instead of <tt>floppy</tt> if you want shorter name.
namespace floppy { // NOLINT(*-concat-nested-namespaces)
  /// \brief Metadata definitions, such as library version or name.
  namespace meta {
    namespace detail {
      // todo: this must be rewritten
      /// \internal
      constexpr auto is_digit(char c) -> bool { return c <= '9' && c >= '0'; }

      /// \internal
      constexpr auto stoi_impl(char const* str, int value = 0) -> int {
        return *str ? is_digit(*str)
          ? stoi_impl(str + 1, (*str - '0') + value * 10)
          : throw "compile time error: not a digit" : value;
      }

      /// \internal
      constexpr auto stoi(char const* str) -> int { return stoi_impl(str); }
    } // namespace detail

    /// \brief Version number structure.
    /// \headerfile floppy/floppy.h
    /// \ingroup helpers
    /// \see project_meta
    class version
    {
     public:
      /// \brief Creates version object at compile-time.
      /// \param major Version number major as an integer.
      /// \param minor Version number minor as an integer.
      /// \param patch Version number patch as an integer.
      constexpr version(int major, int minor, int patch)
        : major_(major)
        , minor_(minor)
        , patch_(patch)
      {}

      /// \brief Creates version object from string.
      /// \param version Version number as a string.
      constexpr explicit version(std::string_view version)
      {
        auto major = version.substr(0, version.find('.'));
        auto minor = version.substr(version.find('.') + 1, version.find('.', version.find('.') + 1));
        auto patch = version.substr(version.find('.', version.find('.') + 1) + 1);
        this->major_ = detail::stoi(major.data());
        this->minor_ = detail::stoi(minor.data());
        this->patch_ = detail::stoi(patch.data());
      }

      /// \brief Returns constant expression version number major as an integer.
      /// \see minor
      /// \see patch
      [[nodiscard]] constexpr auto major() const noexcept -> int { return this->major_; }

      /// \brief Returns constant expression version number minor as an integer.
      /// \see major
      /// \see patch
      [[nodiscard]] constexpr auto minor() const noexcept -> int { return this->minor_; }

      /// \brief Returns constant expression version number patch as an integer.
      /// \see major
      /// \see minor
      [[nodiscard]] constexpr auto patch() const noexcept -> int { return this->patch_; }

      /// \brief Returns version as a string.
      /// \see major
      /// \see minor
      /// \see patch
      [[nodiscard]] auto as_str() const noexcept -> std::string {
        return std::to_string(this->major_) + "." + std::to_string(this->minor_) + "." + std::to_string(this->patch_);
      }

     private:
      int major_{}; ///< Major version.
      int minor_{}; ///< Minor version.
      int patch_{}; ///< Patch version.
    };

    static_assert(version(1, 2, 3).major() == 1);
    static_assert(version(1, 2, 3).minor() == 2);
    static_assert(version(1, 2, 3).patch() == 3);

    /// \brief Project metadata constexpr structure.
    /// \headerfile floppy/floppy.h
    /// \ingroup helpers
    /// \see version
    class [[maybe_unused]] project_meta
    {
     public:
      /// \brief Creates version object at compile-time.
      /// \param version Project version.
      /// \param name Project name.
      /// \param domain Project domain.
      /// \param organization Project organization.
      /// \see project_name
      consteval project_meta(class version version, std::string_view name, std::string_view domain, std::string_view organization)
        : version_(version)
        , name_(name)
        , domain_(domain)
        , organization_(organization)
      {}

      /// \brief Returns constant expression project version.
      [[nodiscard]] constexpr auto version() const noexcept -> version { return this->version_; }

      /// \brief Returns constant expression project name.
      [[nodiscard]] constexpr auto name() const noexcept -> std::string_view { return this->name_; }

      /// \brief Returns constant expression project domain.
      [[nodiscard]] constexpr auto domain() const noexcept -> std::string_view { return this->domain_; }

      /// \brief Returns constant expression project organization.
      [[nodiscard]] constexpr auto organization() const noexcept -> std::string_view { return this->organization_; }

     private:
      class version version_;        ///< Version number.
      std::string_view name_;        ///< Project name.
      std::string_view domain_;      ///< Project domain.
      std::string_view organization_;///< Project organization.
    };

    /// \brief Project metadata object, available at compile-time.
    /// \headerfile floppy/floppy.h
    /// \ingroup helpers
    /// \bug When building via Conan, this variable can be defined with default values instead of
    /// actual version and target name. This will be fixed in the future releases.
    [[maybe_unused]] constexpr inline auto floppy_meta = project_meta(
#if defined(FLOPPY_PROJECT_VERSION_MAJOR)
      version(
        FLOPPY_PROJECT_VERSION_MAJOR,
        FLOPPY_PROJECT_VERSION_MINOR,
        FLOPPY_PROJECT_VERSION_PATCH
      ),
#else // defined(FLOPPY_PROJECT_VERSION_MAJOR)
      version(0, 0, 0),
#endif // defined(FLOPPY_PROJECT_VERSION_MAJOR)

#if defined(FLOPPY_TARGET_NAME)
      std::string_view(stringify$(FLOPPY_TARGET_NAME)),
#else // defined(FLOPPY_TARGET_NAME)
      std::string_view("floppy"),
#endif // defined(FLOPPY_TARGET_NAME)
      "io.github.whs31",
      "whs31"
    );

    /// \brief Stream adaptor for floppy::meta::version
    template <class E, class T>
    auto operator<<(std::basic_ostream<E, T>& os, version const& d) -> std::basic_ostream<E, T>& {
      os << fmt::format("{}.{}.{}", d.major(), d.minor(), d.patch());
      return os;
    }

    /// \brief Stream adaptor for floppy::meta::project_meta
    template <class E, class T>
    auto operator<<(std::basic_ostream<E, T>& os, project_meta const& d) -> std::basic_ostream<E, T>& {
      os << fmt::format("{} v. {}.{}.{} (c) {} <{}>",
        d.name(),
        d.version().major(),
        d.version().minor(),
        d.version().patch(),
        d.organization(),
        d.domain()
      );
      return os;
    }

#if defined(FLOPPY_TARGET_NAME)
    static_assert(floppy_meta.version().major() == FLOPPY_PROJECT_VERSION_MAJOR, "major version isn't the same");
    static_assert(floppy_meta.version().minor() == FLOPPY_PROJECT_VERSION_MINOR, "minor version isn't the same");
    static_assert(floppy_meta.version().patch() == FLOPPY_PROJECT_VERSION_PATCH, "patch version isn't the same");
    static_assert(floppy_meta.name() == std::string_view(stringify$(FLOPPY_TARGET_NAME)), "project name isn't the same");
    static_assert(floppy_meta.domain() == "io.github.whs31", "project domain isn't the same");
    static_assert(floppy_meta.organization() == "whs31", "project organization isn't the same");
#endif // defined(FLOPPY_TARGET_NAME)
  } // namespace meta
} // namespace floppy

#undef _stringify$
#undef stringify$

/// \namespace fl
/// \brief Alias for the main namespace \ref floppy.
/// \headerfile floppy/floppy.h
/// \ingroup aliases
namespace fl = floppy; // NOLINT(*-unused-alias-decls)

#if defined(QT_CORE_LIB) || __has_include("qtglobal.h") || __has_include("qcoreapplication.h") || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup macros
/// \brief Flag defined if <b>Qt::Core</b> library is available and linked.
# define FL_QT_CORE
#endif // defined(QT_CORE_LIB)

#if defined(QT_GUI_LIB) || __has_include("qpainter.h") || __has_include("qguiapplication.h") || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup macros
/// \brief Flag defined if <b>Qt::Gui</b> library is available and linked.
# define FL_QT_GUI
#endif // defined(QT_GUI_LIB)

#if defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup macros
/// \brief Flag defined if documentation is being generated.
# define FL_DOC
#endif // defined(DOXYGEN_GENERATING_OUTPUT)

#if defined(NDEBUG) || defined(QT_NO_DEBUG)
# define FL_NO_DEBUG
#else // defined(NDEBUG) || defined(QT_NO_DEBUG)
# define FL_DEBUG
#endif // defined(NDEBUG)  || defined(QT_NO_DEBUG)

/// \def FL_NO_DEBUG
/// \brief Flag defined if debug mode is disabled.
/// \details This macro disables some runtime checks, which are enabled in debug mode.
/// Be sure to test your code in debug mode to avoid unexpected behavior.
/// \ingroup macros

/// \def FL_DEBUG
/// \brief Flag defined if debug mode is enabled.
/// \ingroup macros