#pragma once

#include "../../types/u128.h"
#include "../../types/fixed_string.h"
#include "../../types/optional.h"

namespace rolly::net {
  /**
   * @brief IP address versions.
   */
  enum class ip_version {
    v4 = 4,  ///< IPv4 version identifier.
    v6 = 6   ///< IPv6 version identifier.
  };

  /**
   * @brief Enumerates the formatting options for IP address strings.
   * @details Defines the formatting styles that can be applied when converting IP addresses to
   * strings, such as full, compact, or compressed representations.
   */
  enum class format {
    full = 0,   ///< Full format with no compression or omission.
    compact,    ///< Compact format with possible omission of leading zeros.
    compressed  ///< Compressed format with maximal omission of segments or octets.
  };

  /**
   * @brief A template base class for IP address representations.
   * @details This class template serves as a base for creating IP address objects. It
   * inherits from a base class that provides the necessary functionality, and
   * it is extended by more specific IP address classes.
   * @tparam Base The base class providing storage and low-level IP address functionalities.
   */
  template <typename Base>
  class ip_address_base : public Base {
   public:
    using base_type = typename Base::base_type;  ///< Type alias for the base storage type.
    using uint_type =
      typename Base::uint_type;  ///< Type alias for the underlying unsigned integer type.

    /**
     * @brief Default constructor.
     * @details Constructs an empty IP address object.
     */
    constexpr ___inline___ ip_address_base() noexcept
      : Base() {}

    /**
     * @brief Constructs an IP address object from a byte array.
     * @param[in] bytes The byte array representing an IP address.
     */
    constexpr ___inline___ explicit ip_address_base(base_type const& bytes) noexcept
      : Base(bytes) {}

    /**
     * @brief Creates an IP address object from a byte array.
     * @param[in] bytes The byte array representing an IP address.
     * @return An instance of ip address constructed from the given byte array.
     */
    [[nodiscard]] static constexpr ___inline___ ip_address_base
      from_bytes(base_type const& bytes) noexcept {
      return ip_address_base(bytes);
    }

    /**
     * @brief Creates an IP address object from a raw byte buffer.
     * @param[in] bytes Pointer to the raw byte buffer representing an IP address.
     * @param[in] byte_count The number of bytes in the buffer to use.
     * @return An instance of ip address constructed from the given byte buffer.
     * @parblock
     * @remark If the number of bytes \a byte_count is less than the target number .
     *         of bytes to represent the IP address, the missing bytes will be
     *         filled with zeros.
     * @endparblock
     * @parblock
     * @remark If the number of bytes \a byte_count is greater than the target .
     *         number of bytes represented by the IP address, then the extra
     *         bytes will be ignored
     * @endparblock
     */
    [[nodiscard]] static constexpr ___inline___ ip_address_base
      from_bytes(uint8_t const* bytes, size_t byte_count) noexcept {
      base_type data = {};
      for(size_t i = 0; i < Base::base_size && i < byte_count; ++i)
        data[i] = bytes[i];
      return ip_address_base(data);
    }

    /**
     * @brief Parses an IP address from a string view.
     * @details This method provides a way to parse an IP address from a string view,
     * which is a non-owning reference to a sequence of characters.
     * @param[in] address The string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings
     * will be used instead.
     */
    [[nodiscard]] static constexpr ___inline___ ip_address_base
      parse(std::string_view const address) noexcept {
      return parse_string(address);
    }

    /**
     * @brief Parses an IP address from a wide string view.
     * @details This method allows for parsing an IP address from a wide string view,
     * which is typically used for wide character strings.
     * @param[in] address The wide string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings
     * will be used instead.
     */
    [[nodiscard]] static constexpr ___inline___ ip_address_base
      parse(std::wstring_view address) noexcept {
      return parse_string(address);
    }

#if __cpp_char8_t >= 201'811L
    /**
     * @brief Parses an IP address from a UTF-8 string view.
     * @details This method parses an IP address from a given UTF-8 string view. It leverages the
     * `char8_t` type introduced in C++20 to handle UTF-8 strings natively.
     * @param[in] address A UTF-8 string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++20 and later versions where `char8_t` is supported.
     */
    [[nodiscard]] static constexpr ___inline___ ip_address_base
      parse(std::u8string_view address) noexcept {
      return parse_string(address);
    }
#endif  // __cpp_char8_t

    /**
     * @brief Parses an IP address from a UTF-16 string view.
     * @details This method is designed to parse an IP address from a UTF-16 string view,
     * which is a sequence of 16-bit characters.
     * @param[in] address The UTF-16 string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings
     * will be used instead.
     */
    [[nodiscard]] static constexpr ___inline___ ip_address_base
      parse(std::u16string_view address) noexcept {
      return parse_string(address);
    }

    /**
     * @brief Parses an IP address from a UTF-32 string view.
     * @details This method allows for parsing an IP address from a UTF-32 string view,
     * which is a sequence of 32-bit characters.
     * @param[in] address The UTF-32 string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings
     * will be used instead.
     */
    [[nodiscard]] static constexpr ___inline___ ip_address_base
      parse(std::u32string_view address) noexcept {
      return parse_string(address);
    }

    /**
     * @brief Parses an IP address from a string view and reports errors through an error code.
     * @details This method parses an IP address from a string view and provides an error code if
     * the parsing fails.
     * @param[in] address The string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs
     * during parsing.
     * @return An instance of ip address parsed from the string view. If parsing fails, the returned
     * object will be in an unspecified state.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings
     * will be used instead.
     */
    static constexpr ___inline___ optional<ip_address_base> try_parse(
      std::string_view address
    ) noexcept {
      auto error = false;
      auto result = parse_string(address, error);
      return error ? none : some(result);
    }

    /**
     * @brief Parses an IP address from a wide string view and reports errors through an error code.
     * @details This method parses an IP address from a wide string view and provides an error code
     * if the parsing fails.
     * @param[in] address The wide string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs
     * during parsing.
     * @return An instance of ip address parsed from the wide string view. If parsing fails, the
     * returned object will be in an unspecified state.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings
     * will be used instead.
     */
    static constexpr ___inline___ optional<ip_address_base> try_parse(
      std::wstring_view address
    ) noexcept {
      auto error = false;
      auto result = parse_string(address, error);
      return error ? none : some(result);
    }

#if __cpp_char8_t >= 201'811L
    /**
     * @brief Parses an IP address from a UTF-8 string view and reports errors through an error
     * code.
     * @details This method parses an IP address from a UTF-8 string view and provides an error code
     * if the parsing fails.
     * @param[in] address The UTF-8 string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs
     * during parsing.
     * @return An instance of ip address parsed from the UTF-8 string view. If parsing fails, the
     * returned object will be in an unspecified state.
     * @note This method is available for C++20 and later versions where `char8_t` is supported.
     */
    static constexpr ___inline___ optional<ip_address_base> try_parse(
      std::u8string_view address
    ) noexcept {
      auto error = false;
      auto result = parse_string(address, error);
      return error ? none : some(result);
    }
#endif  // __cpp_char8_t

    /**
     * @brief Parses an IP address from a UTF-16 string view and reports errors through an error
     * code.
     * @details This method parses an IP address from a UTF-16 string view and provides an error
     * code if the parsing fails.
     * @param[in] address The UTF-16 string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs
     * during parsing.
     * @return An instance of ip address parsed from the UTF-16 string view. If parsing fails, the
     * returned object will be in an unspecified state.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings
     * will be used instead.
     */
    static constexpr ___inline___ optional<ip_address_base> try_parse(
      std::u16string_view address
    ) noexcept {
      auto error = false;
      auto result = parse_string(address, error);
      return error ? none : some(result);
    }

    /**
     * @brief Parses an IP address from a UTF-32 string view and reports errors through an error
     * code.
     * @details This method parses an IP address from a UTF-32 string view and provides an error
     * code if the parsing fails.
     * @param[in] address The UTF-32 string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs
     * during parsing.
     * @return An instance of ip address parsed from the UTF-32 string view. If parsing fails, the
     * returned object will be in an unspecified state.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings
     * will be used instead.
     */
    static constexpr ___inline___ optional<ip_address_base> try_parse(
      std::u32string_view address
    ) noexcept {
      auto error = false;
      auto result = parse_string(address, error);
      return error ? none : some(result);
    }

    /**
     * @brief Parses an IP address from a character array.
     * @details This method template parses an IP address from a character array of a
     * specified size. Can check and get the result at compile time.
     * @tparam T The character type of the array.
     * @tparam N The size of the character array.
     * @param[in] address The character array containing the IP address to parse.
     * @return An instance of ip address parsed from the character array.
     */
    template <typename T, size_t N>
    [[nodiscard]] static constexpr ___inline___ ip_address_base
      parse(T const (&address)[N]) noexcept {
      auto error = false;
      auto str = make_fixed_string(address, error);
      if(error)
        return {};
      return parse_string(str);
    }

    /**
     * @brief Parses an IP address from a character array and reports errors through an error code.
     * @details This method template parses an IP address from a character array of a specified size
     * and provides an error code if the parsing fails.
     * @tparam T The character type of the array.
     * @tparam N The size of the character array.
     * @param[in] address The character array containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs
     * during parsing.
     * @return An instance of ip address parsed from the character array. If parsing fails, the
     * returned object will be in an unspecified state.
     */
    template <typename T, size_t N>
    static constexpr ___inline___ optional<ip_address_base> try_parse(
      T const (&address)[N]
    ) noexcept {
      auto error = false;
      auto str = make_fixed_string(address, error);
      if(error)
        return none;
      auto res = parse_string(str, error);
      return error ? none : some(res);
    }

    /**
     * @brief Retrieves the raw data representing the IP address in **network byte order**
     * (big-endian).
     * @details This method returns a pointer to the underlying byte array that stores the IP
     * address.
     * @return A pointer to the constant byte array containing the raw IP address data.
     */
    [[nodiscard]] constexpr ___inline___ const u8* data() const noexcept {
      return Base::bytes().data();
    }

    /**
     * @brief Checks if the IP address is a multicast address.
     * @return `true` if the IP address is reserved for multicast use, `false` otherwise.
     * @see [RFC 3171](https://datatracker.ietf.org/doc/html/rfc3171.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    [[nodiscard]] constexpr ___inline___ bool is_multicast() const noexcept;

    /**
     * @brief Checks if the IP address is a private address.
     * @return `true` if the IP address is allocated for private networks, `false` otherwise.
     * @see
     * [iana-ipv4-special-registry](https://www.iana.org/assignments/iana-ipv4-special-registry/iana-ipv4-special-registry.xhtml).
     * @see
     * [iana-ipv6-special-registry](https://www.iana.org/assignments/iana-ipv6-special-registry/iana-ipv6-special-registry.xhtml).
     */
    [[nodiscard]] constexpr ___inline___ bool is_private() const noexcept;

    /**
     * @brief Checks if the IP address is a global address.
     * @return `true` if the IP address is allocated for public networks, `false` otherwise.
     * @see
     * [iana-ipv4-special-registry](https://www.iana.org/assignments/iana-ipv4-special-registry/iana-ipv4-special-registry.xhtml).
     * @see
     * [iana-ipv6-special-registry](https://www.iana.org/assignments/iana-ipv6-special-registry/iana-ipv6-special-registry.xhtml).
     */
    [[nodiscard]] constexpr ___inline___ bool is_global() const noexcept;

    /**
     * @brief Checks if the IP address is a reserved address.
     * @return `true` if the IP address is otherwise IETF reserved, `false` otherwise.
     */
    [[nodiscard]] constexpr ___inline___ bool is_reserved() const noexcept;

    /**
     * @brief Checks if the IP address is a loopback address.
     * @return `true` if the IP address is a loopback address, `false` otherwise.
     * @see [RFC 3330](https://datatracker.ietf.org/doc/html/rfc3330.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    [[nodiscard]] constexpr ___inline___ bool is_loopback() const noexcept;

    /**
     * @brief Checks if the IP address is a link-local address.
     * @return `true` if the IP address is reserved for link-local usage, `false` otherwise.
     * @see [RFC 3927](https://datatracker.ietf.org/doc/html/rfc3927.html).
     */
    [[nodiscard]] constexpr ___inline___ bool is_link_local() const noexcept;

    /**
     * @brief Checks if the IP address is unspecified.
     * An unspecified IP address is an address with all bits set to zero.
     * In IPv4, this is represented as 0.0.0.0, and in IPv6, as ::.
     * This type of address is used to indicate the absence of an address.
     * @return `true` if the IP address is unspecified (all bits are zero), `false` otherwise.
     * @see [RFC 5735](https://datatracker.ietf.org/doc/html/rfc5735.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    [[nodiscard]] constexpr ___inline___ bool is_unspecified() const noexcept {
      auto const& b = Base::bytes();
      for(size_t i = 0; i < Base::base_size; ++i)
        if(b[i] != 0)
          return false;
      return true;
    }

    /**
     * @brief Converts the IP address to a string representation.
     * @details The function converts the binary representation of the IP address to a string.
     * The format of the output string can be adjusted by passing the desired format as an argument.
     * The default format is 'compressed'.
     * @param[in] fmt The format to use for the string representation. Defaults to
     * `format::compressed`.
     * @return A `std::string` representing the IP address in the specified format.
     */
    [[nodiscard]] ___inline___ std::string to_string(format fmt = format::compressed) const {
      char res[Base::base_max_string_len + 1] {};
      auto const len = Base::ip_to_chars(Base::bytes(), fmt, res);
      return std::string(res, len);
    }

    /**
     * @brief Swaps the contents of this IP address with another IP address.
     * @param[in] ip The other IP address to swap with.
     */
    constexpr ___inline___ void swap(ip_address_base& ip) noexcept { Base::swap(*this, ip); }

    /**
     * @brief Computes a hash value for the IP address.
     * @return A `size_t` hash value of the IP address.
     */
    [[nodiscard]] constexpr ___inline___ std::size_t hash() const noexcept {
      return Base::hash(Base::bytes());
    }

    /**
     * @brief Generates a reverse DNS lookup pointer for the IP address.
     * @details This function creates a string that is the reverse DNS lookup pointer of the IP
     * address. It is commonly used in reverse DNS lookups, where the IP address is reversed and
     * appended with `.in-addr.arpa` for IPv4, or `.ip6.arpa` for IPv6, to form a domain name that
     * can be looked up in the DNS system. The name of the reverse DNS PTR record for the IP
     * address, e.g.:
     * @code{.cpp}
     *   std::cout << ipv4_address::parse("127.0.0.1").reverse_pointer() << std::endl;
     *   std::cout << ipv6_address::parse("2001:db8::1").reverse_pointer() << std::endl;
     *
     *   // out:
     *   // 1.0.0.127.in-addr.arpa
     *   // 1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.8.b.d.0.1.0.0.2.ip6.arpa
     * @endcode
     * @return A `std::string` that is the reverse DNS lookup pointer of the IP address.
     */
    [[nodiscard]] ___inline___ std::string reverse_pointer() const {
      return Base::ip_reverse_pointer(Base::bytes());
    }

    /**
     * @brief Converts the IP address to an unsigned integer type.
     * @details This operator allows the IP address to be used as an unsigned integer type.
     * It can be useful when a numerical representation of the IP address is needed.
     * @return An unsigned integer representation of the IP address.
     * @remark Bytes in integer are presented in **host byte order**.
     */
    [[nodiscard]] explicit constexpr ___inline___ operator uint_type() const noexcept {
      return Base::to_uint();
    }

    [[nodiscard]] constexpr ___inline___ bool operator==(ip_address_base const& rhs
    ) const noexcept {
      return Base::equals(*this, rhs);
    }

    [[nodiscard]] constexpr ___inline___ bool operator!=(ip_address_base const& rhs
    ) const noexcept {
      return ! (*this == rhs);
    }

    [[nodiscard]] constexpr ___inline___ bool operator<(ip_address_base const& rhs) const noexcept {
      return Base::less(*this, rhs);
    }

    [[nodiscard]] constexpr ___inline___ bool operator>(ip_address_base const& rhs) const noexcept {
      return rhs < *this;
    }

    [[nodiscard]] constexpr ___inline___ bool operator<=(ip_address_base const& rhs
    ) const noexcept {
      return ! (rhs < *this);
    }

    [[nodiscard]] constexpr ___inline___ bool operator>=(ip_address_base const& rhs
    ) const noexcept {
      return ! (*this < rhs);
    }

   private:
    template <typename>
    friend class ip_network_base;

    template <typename Str>
    [[nodiscard]] static constexpr ___inline___ ip_address_base parse_string(Str const& address) {
      auto error = false;
      u32 value = 0;
      auto result =
        Base::ip_from_string(address.data(), address.data() + address.size(), error, value);
      if(error)
        throw std::domain_error("failed to parse IP address");
      return result;
    }

    template <typename Str>
    static constexpr ___inline___ ip_address_base
      parse_string(Str const& address, bool& error) noexcept {
      error = false;
      u32 value = 0;
      return Base::ip_from_string(address.data(), address.data() + address.size(), error, value);
    }
  };

  namespace detail {
    template <typename Base, typename TChar, size_t MaxLen>
    [[nodiscard]] constexpr ___inline___ ip_address_base<Base>
      parse_ip_from_literal(TChar const* address, size_t size) {
      if(size > MaxLen)
        throw std::invalid_argument("invalid IP address literal: string is too long");
      TChar str[MaxLen + 1] = {};
      for(size_t i = 0; i < size && i < MaxLen; ++i)
        str[i] = address[i];
      return ip_address_base<Base>::parse(str);
    }
  }  // namespace detail

  [[nodiscard]] ___inline___ int stream_index() {
    static auto const i = std::ios_base::xalloc();
    return i;
  }

  template <typename T>
  ___inline___ std::basic_ostream<T, std::char_traits<T>>& full(
    std::basic_ostream<T, std::char_traits<T>>& stream
  ) {
    stream.iword(stream_index()) = static_cast<long>(format::full) + 1;
    return stream;
  }

  template <typename T>
  ___inline___ std::basic_ostream<T, std::char_traits<T>>& compact(
    std::basic_ostream<T, std::char_traits<T>>& stream
  ) {
    stream.iword(stream_index()) = static_cast<long>(format::compact) + 1;
    return stream;
  }

  template <typename T>
  ___inline___ std::basic_ostream<T, std::char_traits<T>>& compressed(
    std::basic_ostream<T, std::char_traits<T>>& stream
  ) {
    stream.iword(stream_index()) = static_cast<long>(format::compressed) + 1;
    return stream;
  }
}  // namespace rolly::net

namespace std {
  template <typename Base>
  struct hash<rolly::net::ip_address_base<Base>> {
    [[nodiscard]] constexpr ___inline___ size_t
      operator()(rolly::net::ip_address_base<Base> const& ip) const noexcept {
      return ip.hash();
    }
  };

  template <typename Base>
  constexpr ___inline___ void
    swap(rolly::net::ip_address_base<Base>& ip1, rolly::net::ip_address_base<Base>& ip2) noexcept {
    ip1.swap(ip2);
  }

  template <typename Base>
  [[nodiscard]] ___inline___ std::string to_string(rolly::net::ip_address_base<Base> const& ip) {
    return ip.to_string();
  }

  template <typename Base>
  ___inline___ std::basic_ostream<char, std::char_traits<char>>& operator<<(
    std::basic_ostream<char, std::char_traits<char>>& stream,
    rolly::net::ip_address_base<Base> const& ip
  ) {
    auto& iword = stream.iword(rolly::net::stream_index());
    auto fmt = iword ? static_cast<rolly::net::format>(iword - 1) : rolly::net::format::compressed;
    iword = 0;
    auto str = ip.to_string(fmt);
    if(stream.flags() & ios_base::uppercase) {
      auto end = std::find(str.cbegin(), str.cend(), '%');
      std::transform(str.cbegin(), end, str.begin(), [](char c) { return std::toupper(c); });
    }
    return stream << str;
  }

  template <typename Base>
  ___inline___ std::basic_istream<char, std::char_traits<char>>& operator>>(
    std::basic_istream<char, std::char_traits<char>>& stream,
    rolly::net::ip_address_base<Base>& ip
  ) {
    std::basic_string<char, std::char_traits<char>, std::allocator<char>> str;
    stream >> str;
    bool err = false;
    ip = rolly::net::ip_address_base<Base>::parse(str, err);
    if(err)
      stream.setstate(std::ios_base::failbit);
    return stream;
  }
}  // namespace std
