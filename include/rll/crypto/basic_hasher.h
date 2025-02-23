#pragma once

#include <string>
#include <type_traits>
#include <rll/global/definitions.h>

#ifdef _MSC_VER
#  include <ciso646>
#endif

namespace rll::crypto {
  class basic_hasher {
   public:
    basic_hasher() = default;
    basic_hasher(basic_hasher const&) = default;
    basic_hasher(basic_hasher&&) = default;
    basic_hasher& operator=(basic_hasher const&) = default;
    basic_hasher& operator=(basic_hasher&&) = default;
    virtual ~basic_hasher() = default;

    virtual basic_hasher& append(std::string const& str) = 0;
    virtual basic_hasher& append(std::string_view str) = 0;
    virtual basic_hasher& append(void const* str, std::size_t len) = 0;

    template <
      typename T,
      typename = std::enable_if_t<std::is_standard_layout_v<T> and std::is_trivial_v<T>>>
    basic_hasher& append_raw(T const& value) {
      return this->append(&value, sizeof(T));
    }

    virtual void reset() = 0;

    [[nodiscard]] virtual std::string hash_string() const = 0;

    ___inline___ basic_hasher& operator<<(std::string const& str) { return this->append(str); }

    ___inline___ basic_hasher& operator<<(std::string_view const str) { return this->append(str); }

    template <
      typename T,
      typename = std::enable_if_t<
        std::is_standard_layout_v<T> and std::is_trivial_v<T>
        and not std::is_convertible_v<T, std::string> and not std::is_pointer_v<T>>>
    basic_hasher& operator<<(T const& value) {
      return this->append_raw(value);
    }
  };
}  // namespace rll::crypto
