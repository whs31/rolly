#pragma once

#include <array>
#include <string>
#include <rll/global/definitions.h>
#include <rll/stdint.h>
#include <rll/impl/char_reader.h>

namespace rll {
  /**
   * @brief Fixed size string class.
   * @details fixed_string is a template class that encapsulates a string of a fixed number of
   * characters. It is designed to be used where strings are needed as non-type template parameters.
   * @tparam N The maximum number of characters the fixed_string can hold.
   */
  template <std::size_t N>
  struct fixed_string {
    using value_type = char;            ///< Type of character in a string.
    using const_pointer = char const*;  ///< Type of constant pointer to the string data.
    using const_reference =
      char const&;  ///< Type of constant reference to a character in the string.
    using const_iterator = const_pointer;  ///< Type of constant iterator for traversing the string.
    using const_reverse_iterator = std::reverse_iterator<
      const_iterator>;  ///< Type of constant reverse iterator for traversing the string in reverse.

    static constexpr auto max_length = N;

    size_t length = 0;
    char _data[N] = {};

    /**
     * @brief Default constructor.
     * @details Constructs a fixed_string with default values, initializing the string with null
     * characters.
     */
    constexpr ___inline___ fixed_string() noexcept = default;

    /**
     * @brief Constructs a fixed_string from a character array.
     * @details This constructor template initializes a fixed_string with the contents of a given
     * character array.
     * @tparam T The character type of the input array.
     * @param[in] data The character array to initialize the fixed_string with.
     * @throw parse_error Thrown if contains unexpected characters for addresses
     */
    template <typename T>
    constexpr ___inline___ fixed_string(T const (&data)[N + 1]) noexcept(
      noexcept(detail::char_reader<T>::has_throw())
    )
      : _data() {
      auto const begin = &data[0];
      auto const end = &data[N];
      auto it = begin;
      for(size_t i = 0; i < N; ++i) {
        this->_data[i] = detail::next_char(it, begin, end);
        if(this->_data[i] == '\0')
          break;
        ++length;
      }
    }

    /**
     * @brief Constructs a fixed_string from a character array.
     * @details This constructor template initializes a fixed_string with the contents of a given
     * character array.
     * @tparam T The character type of the input array.
     * @param[in] data The character array to initialize the fixed_string with.
     * @param[out] error A reference to an `bool` object that will be set to `true` if an error
     * occurs during parsing.
     */
    template <typename T>
    constexpr ___inline___ fixed_string(T const (&data)[N + 1], bool& error) noexcept {
      auto const begin = &data[0];
      auto const end = &data[N];
      auto it = begin;
      u32 error_symbol = 0;
      for(size_t i = 0; i < N; ++i) {
        this->_data[i] = detail::next_char_or_error(it, end, error, error_symbol);
        if(this->_data[i] == '\0' or error)
          break;
        ++length;
      }
    }

    /**
     * @brief Retrieves the begin iterator of the fixed_string.
     * @details Returns an iterator pointing to the first character of the fixed_string.
     * If the fixed_string is empty, the returned iterator will be equal to the one returned by
     * end().
     * @return A constant iterator to the beginning of the fixed_string.
     * @sa     end()
     */
    [[nodiscard]] constexpr ___inline___ const_iterator begin() const noexcept {
      return static_cast<const_iterator>(this->_data);
    }

    /**
     * @brief Retrieves the end iterator of the fixed_string.
     * @details Returns an iterator pointing to the past-the-end character of the fixed_string.
     * This iterator acts as a placeholder and should not be dereferenced.
     * @return A constant iterator to the element following the last character.
     * @sa     begin()
     */
    [[nodiscard]] constexpr ___inline___ const_iterator end() const noexcept {
      return static_cast<const_iterator>(this->_data) + this->length;
    }

    /**
     * @brief Retrieves the reverse begin iterator of the fixed_string.
     * @details Returns a reverse iterator pointing to the last character of the fixed_string.
     * If the fixed_string is empty, the returned iterator will be equal to rend().
     * @return A constant reverse iterator to the beginning of the reversed fixed_string.
     * @sa    rend()
     */
    [[nodiscard]] constexpr ___inline___ const_reverse_iterator rbegin() const noexcept {
      return const_reverse_iterator(this->end());
    }

    /**
     * @brief Retrieves the reverse end iterator of the fixed_string.
     * @details Returns a reverse iterator pointing to the position preceding the first character of
     * the fixed_string. This iterator acts as a placeholder and should not be dereferenced.
     * @return A constant reverse iterator to the end of the reversed fixed_string.
     * @sa     rbegin()
     */
    [[nodiscard]] constexpr ___inline___ const_reverse_iterator rend() const noexcept {
      return const_reverse_iterator(this->begin());
    }

    /**
     * @brief Retrieves the constant begin iterator of the fixed_string.
     * @details Returns a constant iterator pointing to the first character of the fixed_string.
     * If the fixed_string is empty, the returned iterator will be equal to cend().
     * @return A constant iterator to the beginning of the fixed_string.
     * @sa     cend()
     */
    [[nodiscard]] constexpr ___inline___ const_iterator cbegin() const noexcept {
      return this->begin();
    }

    /**
     * @brief Retrieves the constant end iterator of the fixed_string.
     * @details Returns a constant iterator pointing to the past-the-end character of the
     * fixed_string. This iterator acts as a placeholder and should not be dereferenced.
     * @return A constant iterator to the end of the fixed_string.
     * @sa     cbegin()
     */
    [[nodiscard]] constexpr ___inline___ const_iterator cend() const noexcept {
      return this->end();
    }

    /**
     * @brief Retrieves the constant reverse begin iterator of the fixed_string.
     * @details Returns a constant reverse iterator pointing to the last character of the
     * fixed_string. If the fixed_string is empty, the returned iterator will be equal to crend().
     * @return A constant reverse iterator to the beginning of the reversed fixed_string.
     * @sa     crend()
     */
    [[nodiscard]] constexpr ___inline___ const_reverse_iterator crbegin() const noexcept {
      return const_reverse_iterator(this->cend());
    }

    /**
     * @brief Retrieves the constant reverse end iterator of the fixed_string.
     * @details Returns a reverse iterator pointing to the position preceding the first character of
     * the fixed_string when reversed. This iterator acts as a placeholder and should not be
     * dereferenced.
     * @return A constant reverse iterator to the end of the reversed fixed_string.
     * @sa     crbegin()
     */
    [[nodiscard]] constexpr ___inline___ const_reverse_iterator crend() const noexcept {
      return const_reverse_iterator(this->cbegin());
    }

    /**
     * @brief Checks if the fixed_string is empty.
     * @details Evaluates whether the fixed_string contains no characters.
     * @return `true` if the fixed_string is empty, `false` otherwise.
     */
    [[nodiscard]] constexpr ___inline___ bool empty() const noexcept { return this->size() == 0; }

    /**
     * @brief Retrieves the size of the fixed_string.
     * @details Returns the number of characters currently stored in the fixed_string.
     * @return The number of characters in the fixed_string.
     * @sa     capacity()
     */
    [[nodiscard]] constexpr ___inline___ size_t size() const noexcept { return this->length; }

    /**
     * @brief Retrieves the capacity of the fixed_string.
     * @details Returns the total number of characters that the fixed_string can hold.
     * @return The capacity of the fixed_string.
     * @sa     size()
     */
    [[nodiscard]] constexpr ___inline___ size_t capacity() const noexcept {
      return this->max_length;
    }

    /**
     * @brief Accesses the character at the specified location with bounds checking.
     * @details Returns a reference to the character at the specified location \a n. If \a n is out
     * of bounds, an exception of type `std::out_of_range` will be thrown.
     * @param[in] n The position of the character to return.
     * @return A reference to the character at the specified location.
     * @throw std::out_of_range When going beyond the bounds of the character array.
     * @sa at(size_t) const
     */
    [[nodiscard]] constexpr ___inline___ const_reference operator[](std::size_t const n
    ) const noexcept {
      return this->at(n);
    }

    /**
     * @brief Accesses the character at the specified location with bounds checking.
     * @details Returns a reference to the character at the specified location \a n. If \a n is out
     * of bounds, an exception of type `std::out_of_range` will be thrown.
     * @param[in] n The position of the character to return.
     * @return A reference to the character at the specified location.
     * @throw std::out_of_range When going beyond the bounds of the character array.
     * @sa operator[](size_t) const
     */
    [[nodiscard]] constexpr ___inline___ const_reference at(std::size_t n) const noexcept {
      return this->_data[n];
    }

    /**
     * @brief Accesses the first element.
     * @details Provides a reference to the first element in the string.
     * Undefined behavior occurs if this function is called on an empty string.
     * @return Reference to the first element.
     * @sa     back()
     */
    [[nodiscard]] constexpr ___inline___ const_reference front() const noexcept {
      return this->_data[0];
    }

    /**
     * @brief Accesses the last element.
     * @details Provides a reference to the last element in the string.
     * Undefined behavior occurs if this function is called on an empty string.
     * @return Reference to the last element.
     * @sa     front()
     */
    [[nodiscard]] constexpr ___inline___ const_reference back() const noexcept {
      return this->_data[N - 1];
    }

    /**
     * @brief Provides a pointer to the underlying data.
     * @details Returns a pointer to the underlying array serving as the string's storage.
     * The range [data(), data() + size()) is valid even if the string is empty,
     * but the data is not dereferenceable in that case.
     *
     * @return Pointer to the underlying data storage.
     */
    [[nodiscard]] constexpr ___inline___ const_pointer data() const noexcept { return this->_data; }

    /**
     * @brief Compares the string with another fixed_string.
     * @tparam N2 The size of the other fixed_string.
     * @param[in] rhs The other fixed_string to compare with.
     * @return Negative value if less, zero if equal, positive value if greater.
     */
    template <size_t N2>
    [[nodiscard]] constexpr ___inline___ int compare(fixed_string<N2> const& rhs) const noexcept {
      size_t i = 0;
      for(; i < this->size() and i < rhs.size(); ++i) {
        auto const c1 = this->at(i);
        auto const c2 = rhs.at(i);
        if(c1 != c2)
          return static_cast<int>(c1) - static_cast<int>(c2);
      }

      if(i == this->size() and i == rhs.size())
        return 0;
      else if(i == this->size() and i < rhs.size())
        return -1;
      else
        return 1;
    }

    /**
     * @brief Calculates the hash of the string.
     * @details Computes a hash value for the string using a FNV-1a hash function.
     * @return The hash value of the string.
     */
    [[nodiscard]] constexpr ___inline___ std::size_t hash() const noexcept {
      std::size_t value {};
      std::size_t prime {};

      if constexpr(sizeof(std::size_t) == 8) {
        value = static_cast<std::size_t>(14'695'981'039'346'656'037ULL);
        prime = static_cast<std::size_t>(1'099'511'628'211ULL);
      } else {
        value = static_cast<std::size_t>(2'166'136'261U);
        prime = static_cast<std::size_t>(16'777'619U);
      }

      for(std::size_t i = 0; i < size(); ++i) {
        value ^= static_cast<std::size_t>(_data[i]);
        value *= prime;
      }
      return value;
    }

    /**
     * @brief Swaps the contents with another fixed_string.
     * @param[in,out] other The other fixed_string to swap with.
     */
    constexpr ___inline___ void swap(fixed_string& other) noexcept {
      auto count = this->size() < other.size() ? other.size() : this->size();
      for(std::size_t i = 0; i < count; ++i) {
        auto tmp = this->_data[i];
        this->_data[i] = other._data[i];
        other._data[i] = tmp;
      }
      auto tmp = this->length;
      this->length = other.length;
      other.length = tmp;
    }
  };  // fixed_string<N>

  template <>
  struct fixed_string<0> {
    using value_type = char;
    using const_pointer = char const*;
    using const_reference = char const&;
    using const_iterator = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr std::size_t max_length = 0;
    static constexpr std::size_t length = 0;

    constexpr ___inline___ fixed_string() noexcept = default;

    constexpr ___inline___ fixed_string(char const*) noexcept {}

    constexpr ___inline___ fixed_string(wchar_t const*) noexcept {}

    constexpr ___inline___ fixed_string(char16_t const*) noexcept {}

    constexpr ___inline___ fixed_string(char32_t const*) noexcept {}

    constexpr ___inline___ fixed_string(char const*, bool& code) noexcept { code = false; }

    constexpr ___inline___ fixed_string(wchar_t const*, bool& code) noexcept { code = false; }

    constexpr ___inline___ fixed_string(char16_t const*, bool& code) noexcept { code = false; }

    constexpr ___inline___ fixed_string(char32_t const*, bool& code) noexcept { code = false; }

#if __cpp_char8_t >= 201'811L
    constexpr ___inline___ fixed_string(const char8_t*) noexcept {}

    constexpr ___inline___ fixed_string(char8_t const*, bool& code) noexcept { code = false; }
#endif  // __cpp_char8_t

    [[nodiscard]] constexpr ___inline___ const_iterator begin() const noexcept {
      return static_cast<const_iterator>(nullptr);
    }

    [[nodiscard]] constexpr ___inline___ const_iterator end() const noexcept {
      return static_cast<const_iterator>(nullptr) + this->length;
    }

    [[nodiscard]] constexpr ___inline___ const_reverse_iterator rbegin() const noexcept {
      return const_reverse_iterator(this->end());
    }

    [[nodiscard]] constexpr ___inline___ const_reverse_iterator rend() const noexcept {
      return const_reverse_iterator(this->begin());
    }

    [[nodiscard]] constexpr ___inline___ const_iterator cbegin() const noexcept {
      return this->begin();
    }

    [[nodiscard]] constexpr ___inline___ const_iterator cend() const noexcept {
      return this->end();
    }

    [[nodiscard]] constexpr ___inline___ const_reverse_iterator crbegin() const noexcept {
      return const_reverse_iterator(this->cend());
    }

    [[nodiscard]] constexpr ___inline___ const_reverse_iterator crend() const noexcept {
      return const_reverse_iterator(this->cbegin());
    }

    [[nodiscard]] constexpr ___inline___ bool empty() const noexcept { return true; }

    [[nodiscard]] constexpr ___inline___ std::size_t size() const noexcept { return 0; }

    [[nodiscard]] constexpr ___inline___ std::size_t capacity() const noexcept { return 0; }

    [[nodiscard]] constexpr ___inline___ const_reference operator[](std::size_t n) const noexcept {
      return this->at(n);
    }

    [[nodiscard]] constexpr ___inline___ const_reference at(std::size_t /*n*/) const noexcept {
      return this->front();
    }

    [[nodiscard]] constexpr ___inline___ const_reference front() const noexcept {
      return *this->cbegin();
    }

    [[nodiscard]] constexpr ___inline___ const_reference back() const noexcept {
      return *this->cbegin();
    }

    [[nodiscard]] constexpr ___inline___ const_pointer data() const noexcept { return nullptr; }

    template <size_t N2>
    [[nodiscard]] constexpr ___inline___ int compare(fixed_string<N2> const& rhs) const noexcept {
      return rhs.empty() ? 0 : -1;
    }

    [[nodiscard]] constexpr ___inline___ size_t hash() const noexcept { return 0; }

    constexpr ___inline___ void swap(fixed_string& other) noexcept {}
  };  // fixed_string<0>

  /**
   * @brief Compares the contents of two fixed strings for equality.
   * @details Checks if the contents of \a lhs and \a rhs are equal, meaning they have the same
   * number of elements and each element in \a lhs compares equal with the element in \a rhs at the
   * same position.
   * @tparam N1 The maximum number of characters of \a lhs.
   * @tparam N2 The maximum number of characters of \a rhs.
   * @param[in] lhs The fixed string whose contents to compare.
   * @param[in] rhs The fixed string whose contents to compare.
   * @return `true` if the contents of the strings are equal, `false` otherwise.
   * @relates fixed_string
   */
  template <std::size_t N1, std::size_t N2>
  [[nodiscard]] constexpr ___inline___ bool
    operator==(fixed_string<N1> const& lhs, fixed_string<N2> const& rhs) noexcept {
    return lhs.compare(rhs) == 0;
  }

  /**
   * @brief Compares the contents of two fixed strings for inequality.
   * @details Checks if the contents of \a lhs and \a rhs are not equal, meaning they do not have
   * the same number of elements or there is at least one position at which the elements in \a lhs
   * and \a rhs differ.
   * @tparam N1 The maximum number of characters of \a lhs.
   * @tparam N2 The maximum number of characters of \a rhs.
   * @param[in] lhs The fixed string whose contents to compare.
   * @param[in] rhs The fixed string whose contents to compare.
   * @return `true` if the contents of the strings are not equal, `false` otherwise.
   * @relates fixed_string
   */
  template <std::size_t N1, std::size_t N2>
  [[nodiscard]] constexpr ___inline___ bool
    operator!=(fixed_string<N1> const& lhs, fixed_string<N2> const& rhs) noexcept {
    return ! (lhs == rhs);
  }

  /**
   * @brief Compares the contents of two fixed strings lexicographically.
   * @details Checks if the contents of \a lhs are lexicographically less than the contents of \a
   * rhs.
   * @tparam N1 The maximum number of characters of \a lhs.
   * @tparam N2 The maximum number of characters of \a rhs.
   * @param[in] lhs The fixed string whose contents to compare.
   * @param[in] rhs The fixed string whose contents to compare.
   * @return `true` if the contents of \a lhs are lexicographically less than the contents of \a
   * rhs, `false` otherwise.
   * @relates fixed_string
   */
  template <size_t N1, size_t N2>
  [[nodiscard]] constexpr ___inline___ bool
    operator<(fixed_string<N1> const& lhs, fixed_string<N2> const& rhs) noexcept {
    return lhs.compare(rhs) < 0;
  }

  /**
   * @brief Compares the contents of two fixed strings for greater than relation.
   * @details Determines if the contents of \a lhs are lexicographically greater than the contents
   * of \a rhs.
   * @tparam N1 The maximum number of characters of \a lhs.
   * @tparam N2 The maximum number of characters of \a rhs.
   * @param[in] lhs The fixed string whose contents to compare.
   * @param[in] rhs The fixed string whose contents to compare.
   * @return `true` if \a lhs is lexicographically greater than \a rhs, `false` otherwise.
   * @relates fixed_string
   */
  template <size_t N1, size_t N2>
  [[nodiscard]] constexpr ___inline___ bool
    operator>(fixed_string<N1> const& lhs, fixed_string<N2> const& rhs) noexcept {
    return rhs < lhs;
  }

  /**
   * @brief Compares the contents of two fixed strings for less than or equal relation.
   * @details Determines if the contents of \a lhs are lexicographically less than or equal to the
   * contents of
   * \a rhs.
   * @tparam N1 The maximum number of characters of \a lhs.
   * @tparam N2 The maximum number of characters of \a rhs.
   * @param[in] lhs The fixed string whose contents to compare.
   * @param[in] rhs The fixed string whose contents to compare.
   * @return `true` if \a lhs is lexicographically less than or equal to \a rhs, `false` otherwise.
   * @relates fixed_string
   */
  template <size_t N1, size_t N2>
  [[nodiscard]] constexpr ___inline___ bool
    operator<=(fixed_string<N1> const& lhs, fixed_string<N2> const& rhs) noexcept {
    return ! (rhs < lhs);
  }

  /**
   * @brief Compares the contents of two fixed strings for greater than or equal relation.
   * @details Determines if the contents of \a lhs are lexicographically greater than or equal to
   * the contents of \a rhs.
   * @tparam N1 The maximum number of characters of \a lhs.
   * @tparam N2 The maximum number of characters of \a rhs.
   * @param[in] lhs The fixed string whose contents to compare.
   * @param[in] rhs The fixed string whose contents to compare.
   * @return `true` if \a lhs is lexicographically greater than or equal to \a rhs, `false`
   * otherwise.
   * @relates fixed_string
   */
  template <size_t N1, size_t N2>
  [[nodiscard]] constexpr ___inline___ bool
    operator>=(fixed_string<N1> const& lhs, fixed_string<N2> const& rhs) noexcept {
    return ! (lhs < rhs);
  }

  /**
   * @brief Creates a fixed-length string from a character array.
   * @details Constructs a fixed_string object from a character array, deducing the size
   * automatically.
   * @tparam T The character type of the input array.
   * @tparam N The size of the character array plus one for the null terminator.
   * @param[in] data The character array to initialize the fixed_string with.
   * @return A fixed_string object of size N-1.
   * @relates fixed_string
   */
  template <typename T, std::size_t N>
  [[nodiscard]] constexpr ___inline___ fixed_string<N - 1> make_fixed_string(
    T const (&data)[N]
  ) noexcept(noexcept(fixed_string<N - 1>(data))) {
    return fixed_string<N - 1>(data);
  }

  /**
   * @brief Creates a fixed-length string from a character array.
   * @details Constructs a fixed_string object from a character array, deducing the size
   * automatically.
   * @tparam T The character type of the input array.
   * @tparam N The size of the character array plus one for the null terminator.
   * @param[in] data The character array to initialize the fixed_string with.
   * @param[out] code A reference to an `error_code` object that will be set if an error occurs
   * during parsing.
   * @return A fixed_string object of size N-1.
   * @relates fixed_string
   */
  template <typename T, std::size_t N>
  [[nodiscard]] constexpr ___inline___ fixed_string<N - 1>
    make_fixed_string(T const (&data)[N], bool& code) noexcept {
    return fixed_string<N - 1>(data, code);
  }

#if __cpp_char8_t >= 201'811L
  template <std::size_t N>
  fixed_string(const char8_t (&)[N]) -> fixed_string<N - 1>;
#endif  // __cpp_char8_t

  template <std::size_t N>
  fixed_string(fixed_string<N>) -> fixed_string<N>;
  template <std::size_t N>
  fixed_string(char const (&)[N]) -> fixed_string<N - 1>;
  template <std::size_t N>
  fixed_string(wchar_t const (&)[N]) -> fixed_string<N - 1>;
  template <std::size_t N>
  fixed_string(char16_t const (&)[N]) -> fixed_string<N - 1>;
  template <std::size_t N>
  fixed_string(char32_t const (&)[N]) -> fixed_string<N - 1>;
  fixed_string() -> fixed_string<0>;
}  // namespace rll
