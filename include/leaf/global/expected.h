#pragma once

#include <string>
#include <optional>
#include <leaf/global/result.h>

// note: this will break lf::Err.
#if defined(__cpp_lib_expected) and (__cpp_lib_expected >= 202211L)
# if __has_include(<expected>)
#   include <expected>
# elif __has_include(<experimental/expected>)
#   include <experimental/expected>
# else
#   include <leaf/global/result.h>
# endif
#endif

namespace leaf { // NOLINT(*-concat-nested-namespaces)
  inline namespace types {
#if defined(__cpp_lib_expected) and (__cpp_lib_expected >= 202211L)
    using std::expected;
    using std::unexpected;
#else
    using leaf::expected;
#endif
    using std::optional;

    template <typename T>
    using Result = expected<T, std::string>;

    namespace pmr {
      template <typename T, typename E>
      using Result = expected<T, E>;
    }

    template <typename T>
    using Option = optional<T>;

    template <typename E>
    auto Error(E&& e) -> unexpected<std::decay_t<E>> {
      return unexpected<std::decay_t<E>>(std::forward<E>(e));
    }

    template<typename... Args>
    auto Error(std::string_view format, Args&&... args) -> unexpected<std::decay_t<std::string>> {
      return unexpected<std::decay_t<std::string>>(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
    }

    template <class T>
    auto Ok(T&& t) -> expected<std::decay_t<T>, std::string> {
      return expected<std::decay_t<T>, std::string>(std::forward<T>(t));
    }

    template <class T>
    auto Some(T&& t) -> optional<std::decay_t<T>> {
      return optional<std::decay_t<T>>(std::forward<T>(t));
    }

    inline constexpr auto None = std::nullopt;
  }
}