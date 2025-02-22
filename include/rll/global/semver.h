#pragma once

#include <cstddef>
#include <cstdint>
#include <charconv>
#include <iosfwd>
#include <limits>
#include <stdexcept>
#include <rll/global/char_utils.h>
#include <rll/global/version_definitions.h>
#include <rll/types/optional.h>

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wmissing-braces"
#endif

// NOLINTBEGIN(*-use-designated-initializers, *-pro-bounds-pointer-arithmetic,
// *-explicit-constructor,
// *-exception-escape, *-use-default-member-init, *-else-after-return)

#if __cpp_impl_three_way_comparison >= 201'907L
#  include <compare>
#endif

#ifdef __NVCC__
#  define RLL_SEMVER_CONSTEXPR inline
#else
#  define RLL_SEMVER_CONSTEXPR constexpr
#endif  // __NVCC__

namespace rll {
  enum struct prerelease : std::uint8_t {
    alpha = 0,
    beta = 1,
    rc = 2,
    none = 3
  };

#ifndef DOXYGEN
  struct from_chars_result : std::from_chars_result {
    [[nodiscard]] constexpr operator bool() const noexcept { return ec == std::errc {}; }
  };

  struct to_chars_result : std::to_chars_result {
    [[nodiscard]] constexpr operator bool() const noexcept { return ec == std::errc {}; }
  };

  inline constexpr auto max_version_string_length = static_cast<std::size_t>(29);
#endif

#ifndef DOXYGEN
  namespace detail {
    inline constexpr auto alpha = std::string_view {"alpha", 5};
    inline constexpr auto beta = std::string_view {"beta", 4};
    inline constexpr auto rc = std::string_view {"rc", 2};
    inline constexpr auto min_version_string_length = 5;

    [[nodiscard]] constexpr std::uint8_t length(std::uint16_t x) noexcept {
      if(x < 10)
        return 1;
      if(x < 100)
        return 2;
      if(x < 1'000)
        return 3;
      if(x < 10'000)
        return 4;
      return 5;
    }

    [[nodiscard]] constexpr std::uint8_t length(prerelease t) noexcept {
      if(t == prerelease::alpha)
        return static_cast<std::uint8_t>(alpha.length());
      else if(t == prerelease::beta)
        return static_cast<std::uint8_t>(beta.length());
      else if(t == prerelease::rc)
        return static_cast<std::uint8_t>(rc.length());
      return 0;
    }

    [[nodiscard]] constexpr bool
      equals(char const* first, char const* last, std::string_view str) noexcept {
      for(std::size_t i = 0; first != last && i < str.length(); ++i, ++first)
        if(to_lower(*first) != to_lower(str[i]))
          return false;
      return true;
    }

    [[nodiscard]] constexpr char* to_chars(char* str, std::uint16_t x, bool dot = true) noexcept {
      do {  // NOLINT(*-avoid-do-while)
        *(--str) = static_cast<char>('0' + (x % 10));
        x /= 10;
      } while(x != 0);
      if(dot)
        *(--str) = '.';
      return str;
    }

    [[nodiscard]] constexpr char* to_chars(char* str, prerelease t) noexcept {
      auto const p = t == prerelease::alpha ? alpha
                   : t == prerelease::beta  // NOLINT(*-avoid-nested-conditional-operator)
                     ? beta
                     : t == prerelease::rc  // NOLINT(*-avoid-nested-conditional-operator)
                         ? rc
                         : std::string_view {};
      if(not p.empty()) {
        for(auto it = p.rbegin(); it != p.rend(); ++it)  // NOLINT(*-loop-convert)
          *(--str) = *it;
        *(--str) = '-';
      }
      return str;
    }

    [[nodiscard]] RLL_SEMVER_CONSTEXPR char const*
      from_chars(char const* first, char const* last, std::uint16_t& d) noexcept {
      if(first != last and is_digit(*first)) {
        std::int32_t t = 0;
        for(; first != last and is_digit(*first); ++first)
          t = t * 10 + to_digit(*first);
        if(t <= (std::numeric_limits<std::uint16_t>::max)()) {
          d = static_cast<std::uint16_t>(t);
          return first;
        }
      }
      return nullptr;
    }

    [[nodiscard]] RLL_SEMVER_CONSTEXPR char const*
      from_chars(char const* first, char const* last, optional<std::uint16_t>& d) noexcept {
      if(first != last && is_digit(*first)) {
        std::int32_t t = 0;
        for(; first != last && is_digit(*first); ++first)
          t = t * 10 + to_digit(*first);
        if(t <= (std::numeric_limits<std::uint16_t>::max)()) {
          d = static_cast<std::uint16_t>(t);
          return first;
        }
      }

      return nullptr;
    }

    [[nodiscard]] RLL_SEMVER_CONSTEXPR char const*
      from_chars(char const* first, char const* last, prerelease& p) noexcept {
      if(is_hyphen(*first))
        ++first;
      if(equals(first, last, alpha)) {
        p = prerelease::alpha;
        return first + alpha.length();
      } else if(equals(first, last, beta)) {
        p = prerelease::beta;
        return first + beta.length();
      } else if(equals(first, last, rc)) {
        p = prerelease::rc;
        return first + rc.length();
      }
      return nullptr;
    }

    [[nodiscard]] constexpr bool
      check_delimiter(char const* first, char const* last, char d) noexcept {
      return first != last && first != nullptr && *first == d;
    }

    template <typename T, typename = void>
    struct resize_uninitialized {
      static auto resize(T& str, std::size_t size)
        -> std::void_t<decltype(str.resize(size))> {  // NOLINT(*-trailing-return)
        str.resize(size);
      }
    };

    template <typename T>
    struct resize_uninitialized<
      T,
      std::void_t<decltype(std::declval<T>().__resize_default_init(42))>> {
      static void resize(T& str, std::size_t size) { str.__resize_default_init(size); }
    };
  }  // namespace detail
#endif

  struct version {
    std::uint16_t major = 0;
    std::uint16_t minor = 1;
    std::uint16_t patch = 0;
    prerelease prerelease_type = prerelease::none;
    optional<std::uint16_t> prerelease_number = nullopt;

    RLL_SEMVER_CONSTEXPR version(
      std::uint16_t mj,
      std::uint16_t mn,
      std::uint16_t pt,
      prerelease prt = prerelease::none,
      optional<std::uint16_t> prn = nullopt
    ) noexcept
      : major {mj}
      , minor {mn}
      , patch {pt}
      , prerelease_type {prt}
      , prerelease_number {prt == prerelease::none ? nullopt : prn} {}

    RLL_SEMVER_CONSTEXPR
    version(
      std::uint16_t mj,
      std::uint16_t mn,
      std::uint16_t pt,
      prerelease prt,
      std::uint16_t prn
    ) noexcept
      : major {mj}
      , minor {mn}
      , patch {pt}
      , prerelease_type {prt}
      , prerelease_number {prt == prerelease::none ? nullopt : make_optional<std::uint16_t>(prn)} {}

    explicit RLL_SEMVER_CONSTEXPR version(std::string_view str)
      : version(0, 0, 0, prerelease::none, nullopt) {
      from_string(str);
    }

    constexpr version() =
      default;  // https://semver.org/#how-should-i-deal-with-revisions-in-the-0yz-initial-development-phase
    constexpr version(version const&) = default;
    constexpr version(version&&) = default;
    ~version() = default;
    version& operator=(version const&) = default;
    version& operator=(version&&) = default;

    [[nodiscard]] RLL_SEMVER_CONSTEXPR from_chars_result
      from_chars(char const* first, char const* last) noexcept {
      if(first == nullptr or last == nullptr or (last - first) < detail::min_version_string_length)
        return {first, std::errc::invalid_argument};
      auto const* next = first;
      if(next = detail::from_chars(next, last, major); detail::check_delimiter(next, last, '.')) {
        if(next = detail::from_chars(++next, last, minor);
           detail::check_delimiter(next, last, '.')) {
          if(next = detail::from_chars(++next, last, patch); next == last) {
            prerelease_type = prerelease::none;
            prerelease_number = {};
            return {next, std::errc {}};
          } else if(detail::check_delimiter(next, last, '-')) {
            if(next = detail::from_chars(next, last, prerelease_type); next == last) {
              prerelease_number = {};
              return {next, std::errc {}};
            } else if(detail::check_delimiter(next, last, '.'))
              if(next = detail::from_chars(++next, last, prerelease_number); next == last) {
                return {next, std::errc {}};
              }
          }
        }
      }

      return {first, std::errc::invalid_argument};
    }

    [[nodiscard]] RLL_SEMVER_CONSTEXPR to_chars_result
      to_chars(char* first, char* last) const noexcept {
      auto const length = string_length();
      if(first == nullptr or last == nullptr or (last - first) < length)
        return {last, std::errc::value_too_large};
      auto* next = first + length;
      if(prerelease_type != prerelease::none) {
        if(prerelease_number.has_value())
          next = detail::to_chars(next, prerelease_number.value());
        next = detail::to_chars(next, prerelease_type);
      }
      next = detail::to_chars(next, patch);
      next = detail::to_chars(next, minor);
      next = detail::to_chars(next, major, false);

      return {first + length, std::errc {}};
    }

    [[nodiscard]] RLL_SEMVER_CONSTEXPR bool from_string_noexcept(std::string_view str) noexcept {
      return from_chars(str.data(), str.data() + str.length());
    }

    RLL_SEMVER_CONSTEXPR version& from_string(std::string_view str) {
      if(not from_string_noexcept(str))
        throw std::invalid_argument("rll::version::from_string: invalid version.");
      return *this;
    }

    [[nodiscard]] std::string to_string() const {
      auto str = std::string();
      detail::resize_uninitialized<std::string>::resize(str, string_length());
      if(not to_chars(str.data(), str.data() + str.length()))
        throw std::invalid_argument("rll::version::to_string: invalid version.");
      return str;
    }

    [[nodiscard]] RLL_SEMVER_CONSTEXPR std::uint8_t string_length() const noexcept {
      auto length = detail::length(major) + detail::length(minor) + detail::length(patch) + 2;
      if(prerelease_type != prerelease::none) {
        length += detail::length(prerelease_type) + 1;
        if(prerelease_number.has_value())
          length += detail::length(prerelease_number.value()) + 1;
      }
      return static_cast<std::uint8_t>(length);
    }

    [[nodiscard]] constexpr int compare(version const& other) const noexcept {
      if(major != other.major)
        return major - other.major;
      if(minor != other.minor)
        return minor - other.minor;
      if(patch != other.patch)
        return patch - other.patch;
      if(prerelease_type != other.prerelease_type)
        return static_cast<std::uint8_t>(prerelease_type)
             - static_cast<std::uint8_t>(other.prerelease_type);
      if(prerelease_number.has_value()) {
        if(other.prerelease_number.has_value())
          return prerelease_number.value() - other.prerelease_number.value();
        return 1;
      } else if(other.prerelease_number.has_value())
        return -1;
      return 0;
    }
  };

  [[nodiscard]] constexpr bool operator==(version const& lhs, version const& rhs) noexcept {
    return lhs.compare(rhs) == 0;
  }

  [[nodiscard]] constexpr bool operator!=(version const& lhs, version const& rhs) noexcept {
    return lhs.compare(rhs) != 0;
  }

  [[nodiscard]] constexpr bool operator>(version const& lhs, version const& rhs) noexcept {
    return lhs.compare(rhs) > 0;
  }

  [[nodiscard]] constexpr bool operator>=(version const& lhs, version const& rhs) noexcept {
    return lhs.compare(rhs) >= 0;
  }

  [[nodiscard]] constexpr bool operator<(version const& lhs, version const& rhs) noexcept {
    return lhs.compare(rhs) < 0;
  }

  [[nodiscard]] constexpr bool operator<=(version const& lhs, version const& rhs) noexcept {
    return lhs.compare(rhs) <= 0;
  }

#if __cpp_impl_three_way_comparison >= 201'907L
  [[nodiscard]] constexpr std::strong_ordering operator<=>(version const& lhs, version const& rhs) {
    auto const compare = lhs.compare(rhs);
    if(compare == 0)
      return std::strong_ordering::equal;
    if(compare > 0)
      return std::strong_ordering::greater;
    return std::strong_ordering::less;
  }
#endif

  inline namespace literals {
    /**
     * @brief Version literal operator.
     * @details Creates a version from a string literal at compile time.
     *
     * Example usage:
     * @code {.cpp}
     * using namespace rll::literals;
     *
     * auto constexpr v = "1.0.0"_version;
     *
     * static_assert(v == rll::version(1, 0, 0));
     * @endcode
     */
    [[nodiscard]] RLL_SEMVER_CONSTEXPR version
      operator""_version(char const* str, std::size_t length) {
      return version {
        std::string_view {str, length}
      };
    }
  }  // namespace literals

  [[nodiscard]] RLL_SEMVER_CONSTEXPR bool valid(std::string_view str) noexcept {
    return version {}.from_string_noexcept(str);
  }

  [[nodiscard]] RLL_SEMVER_CONSTEXPR from_chars_result
    from_chars(char const* first, char const* last, version& v) noexcept {
    return v.from_chars(first, last);
  }

  [[nodiscard]] RLL_SEMVER_CONSTEXPR to_chars_result
    to_chars(char* first, char* last, version const& v) noexcept {
    return v.to_chars(first, last);
  }

  [[nodiscard]] RLL_SEMVER_CONSTEXPR optional<version> from_string_noexcept(
    std::string_view str
  ) noexcept {
    if(version v {}; v.from_string_noexcept(str))
      return v;
    return nullopt;
  }

  [[nodiscard]] RLL_SEMVER_CONSTEXPR version from_string(std::string_view str) {
    return version {str};
  }

  [[nodiscard]] inline std::string to_string(version const& v) { return v.to_string(); }

  template <typename Char, typename Traits>
  inline std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os, version const& v) {
    for(auto const c : v.to_string())
      os.put(c);
    return os;
  }

  inline namespace comparators {
    enum struct comparators_option : std::uint8_t {
      exclude_prerelease,
      include_prerelease
    };

    [[nodiscard]] constexpr int compare(
      version const& lhs,
      version const& rhs,
      comparators_option option = comparators_option::include_prerelease
    ) noexcept {
      if(option == comparators_option::exclude_prerelease)
        return version {lhs.major, lhs.minor, lhs.patch}.compare(
          version {rhs.major, rhs.minor, rhs.patch}
        );
      return lhs.compare(rhs);
    }

    [[nodiscard]] constexpr bool equal_to(
      version const& lhs,
      version const& rhs,
      comparators_option option = comparators_option::include_prerelease
    ) noexcept {
      return compare(lhs, rhs, option) == 0;
    }

    [[nodiscard]] constexpr bool not_equal_to(
      version const& lhs,
      version const& rhs,
      comparators_option option = comparators_option::include_prerelease
    ) noexcept {
      return compare(lhs, rhs, option) != 0;
    }

    [[nodiscard]] constexpr bool greater(
      version const& lhs,
      version const& rhs,
      comparators_option option = comparators_option::include_prerelease
    ) noexcept {
      return compare(lhs, rhs, option) > 0;
    }

    [[nodiscard]] constexpr bool greater_equal(
      version const& lhs,
      version const& rhs,
      comparators_option option = comparators_option::include_prerelease
    ) noexcept {
      return compare(lhs, rhs, option) >= 0;
    }

    [[nodiscard]] constexpr bool less(
      version const& lhs,
      version const& rhs,
      comparators_option option = comparators_option::include_prerelease
    ) noexcept {
      return compare(lhs, rhs, option) < 0;
    }

    [[nodiscard]] constexpr bool less_equal(
      version const& lhs,
      version const& rhs,
      comparators_option option = comparators_option::include_prerelease
    ) noexcept {
      return compare(lhs, rhs, option) <= 0;
    }
  }  // namespace comparators

#ifndef DOXYGEN
  namespace detail {
    using namespace rll::detail;

    class range {
     public:
      RLL_SEMVER_CONSTEXPR explicit range(std::string_view str) noexcept
        : parser {str} {}

      RLL_SEMVER_CONSTEXPR bool satisfies(version const& ver, bool include_prerelease) {
        bool const has_prerelease = ver.prerelease_type != prerelease::none;
        do {
          if(is_logical_or_token())
            parser.advance_token(range_token_type::logical_or);
          bool contains = true;
          bool allow_compare = include_prerelease;

          while(is_operator_token() or is_number_token()) {
            auto const range = parser.parse_range();
            bool const equal_without_tags =
              equal_to(range.ver, ver, comparators_option::exclude_prerelease);
            if(has_prerelease && equal_without_tags)
              allow_compare = true;
            if(! range.satisfies(ver)) {
              contains = false;
              break;
            }
          }

          if(has_prerelease) {
            if(allow_compare and contains)
              return true;
          } else if(contains)
            return true;
        } while(is_logical_or_token());
        return false;
      }

     private:
      enum struct range_operator : std::uint8_t {
        less,
        less_or_equal,
        greater,
        greater_or_equal,
        equal
      };

      struct range_comparator {
        range_operator op;
        version ver;

        [[nodiscard]] constexpr bool satisfies(version const& version) const {
          switch(op) {
            case range_operator::equal: return version == ver;
            case range_operator::greater: return version > ver;
            case range_operator::greater_or_equal: return version >= ver;
            case range_operator::less: return version < ver;
            case range_operator::less_or_equal: return version <= ver;
            default: throw std::invalid_argument("rll::version_range unexpected operator.");
          }
        }
      };

      enum struct range_token_type : std::uint8_t {
        none,
        number,
        range_operator,
        dot,
        logical_or,
        hyphen,
        prerelease,
        end_of_line
      };

      struct range_token {
        range_token_type type = range_token_type::none;
        std::uint16_t number = 0;
        range_operator op = range_operator::equal;
        prerelease prerelease_type = prerelease::none;
      };

      struct range_lexer {
        std::string_view text;
        std::size_t pos;

        RLL_SEMVER_CONSTEXPR explicit range_lexer(std::string_view text) noexcept
          : text {text}
          , pos {0} {}

        RLL_SEMVER_CONSTEXPR range_token get_next_token() noexcept {
          while(not end_of_line()) {
            if(is_space(text[pos])) {
              advance(1);
              continue;
            }
            if(is_logical_or(text[pos])) {
              advance(2);
              return {range_token_type::logical_or};
            }
            if(is_operator(text[pos])) {
              auto const op = get_operator();
              return {range_token_type::range_operator, 0, op};
            }
            if(is_digit(text[pos])) {
              auto const number = get_number();
              return {range_token_type::number, number};
            }
            if(is_dot(text[pos])) {
              advance(1);
              return {range_token_type::dot};
            }

            if(is_hyphen(text[pos])) {
              advance(1);
              return {range_token_type::hyphen};
            }
            if(is_letter(text[pos])) {
              auto const prerelease = get_prerelease();
              return {range_token_type::prerelease, 0, range_operator::equal, prerelease};
            }
          }
          return {range_token_type::end_of_line};
        }

        RLL_SEMVER_CONSTEXPR bool end_of_line() const noexcept { return pos >= text.length(); }

        RLL_SEMVER_CONSTEXPR void advance(std::size_t i) noexcept { pos += i; }

        RLL_SEMVER_CONSTEXPR range_operator get_operator() noexcept {
          if(text[pos] == '<') {
            advance(1);
            if(text[pos] == '=') {
              advance(1);
              return range_operator::less_or_equal;
            }
            return range_operator::less;
          } else if(text[pos] == '>') {
            advance(1);
            if(text[pos] == '=') {
              advance(1);
              return range_operator::greater_or_equal;
            }
            return range_operator::greater;
          } else if(text[pos] == '=') {
            advance(1);
            return range_operator::equal;
          }
          return range_operator::equal;
        }

        RLL_SEMVER_CONSTEXPR std::uint16_t get_number() noexcept {
          auto const* const first = text.data() + pos;
          auto const* const last = text.data() + text.length();
          if(std::uint16_t n {}; from_chars(first, last, n) != nullptr) {
            advance(length(n));
            return n;
          }
          return 0;
        }

        RLL_SEMVER_CONSTEXPR prerelease get_prerelease() noexcept {
          auto const* const first = text.data() + pos;
          auto const* const last = text.data() + text.length();
          if(first > last) {
            advance(1);
            return prerelease::none;
          }
          if(prerelease p {}; from_chars(first, last, p) != nullptr) {
            advance(length(p));
            return p;
          }
          advance(1);
          return prerelease::none;
        }
      };

      struct range_parser {
        range_lexer lexer;
        range_token current_token;

        RLL_SEMVER_CONSTEXPR explicit range_parser(std::string_view str)
          : lexer {str}
          , current_token {range_token_type::none} {
          advance_token(range_token_type::none);
        }

        RLL_SEMVER_CONSTEXPR void advance_token(range_token_type token_type) {
          if(current_token.type != token_type)
            throw std::invalid_argument("rll::version_range unexpected token.");
          current_token = lexer.get_next_token();
        }

        RLL_SEMVER_CONSTEXPR range_comparator parse_range() {
          if(current_token.type == range_token_type::number) {
            auto const version = parse_version();
            return {range_operator::equal, version};
          } else if(current_token.type == range_token_type::range_operator) {
            auto const range_operator = current_token.op;
            advance_token(range_token_type::range_operator);
            auto const version = parse_version();
            return {range_operator, version};
          }
          return {range_operator::equal, version {}};
        }

        RLL_SEMVER_CONSTEXPR version parse_version() {
          auto const major = parse_number();
          advance_token(range_token_type::dot);
          auto const minor = parse_number();
          advance_token(range_token_type::dot);
          auto const patch = parse_number();
          prerelease prerelease = prerelease::none;
          optional<std::uint16_t> prerelease_number = nullopt;

          if(current_token.type == range_token_type::hyphen) {
            advance_token(range_token_type::hyphen);
            prerelease = parse_prerelease();
            if(current_token.type == range_token_type::dot) {
              advance_token(range_token_type::dot);
              prerelease_number = parse_number();
            }
          }
          return {major, minor, patch, prerelease, prerelease_number};
        }

        RLL_SEMVER_CONSTEXPR std::uint16_t parse_number() {
          auto const token = current_token;
          advance_token(range_token_type::number);
          return token.number;
        }

        RLL_SEMVER_CONSTEXPR prerelease parse_prerelease() {
          auto const token = current_token;
          advance_token(range_token_type::prerelease);
          return token.prerelease_type;
        }
      };

      [[nodiscard]] constexpr bool is_logical_or_token() const noexcept {
        return parser.current_token.type == range_token_type::logical_or;
      }

      [[nodiscard]] constexpr bool is_operator_token() const noexcept {
        return parser.current_token.type == range_token_type::range_operator;
      }

      [[nodiscard]] constexpr bool is_number_token() const noexcept {
        return parser.current_token.type == range_token_type::number;
      }

      range_parser parser;
    };

    enum struct satisfies_option : std::uint8_t {
      exclude_prerelease,
      include_prerelease
    };

    RLL_SEMVER_CONSTEXPR bool satisfies(
      version const& ver,
      std::string_view str,
      satisfies_option option = satisfies_option::exclude_prerelease
    ) {
      switch(option) {
        case satisfies_option::exclude_prerelease: return detail::range {str}.satisfies(ver, false);
        case satisfies_option::include_prerelease: return detail::range {str}.satisfies(ver, true);
        default: throw std::invalid_argument("rll::version_range unexpected satisfies_option.");
      }
    }
  }  // namespace detail
#endif

  using detail::satisfies;
  using version_range = detail::range;
  using version_range_option = detail::satisfies_option;
}  // namespace rll

/**
 * @brief Specialization of the <code>fmt::formatter</code> for the @ref rll::version class.
 * @relates rll::version
 */
template <>
struct [[maybe_unused]] fmt::formatter<rll::version> : fmt::formatter<std::string_view> {
  template <typename FormatContext>
  auto format(rll::version const& v, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}", v.to_string());
  }
};

// NOLINTEND(*-use-designated-initializers, *-pro-bounds-pointer-arithmetic, *-explicit-constructor,
// *-exception-escape,
// *-use-default-member-init, *-else-after-return)

#if defined(__clang__)
#  pragma clang diagnostic pop
#endif
