/**
 * @file span.h
 * @brief A replacement for std::span
 * @note Based on implementation by Tristan Brindle.
 * Copyright Tristan Brindle 2018.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file ../../LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <array>
#include <type_traits>
#include <stdexcept>
#include "./contracts.h"

// Various feature test macros

#if __cplusplus >= 201'703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201'703L)
#  define TCB_SPAN_HAVE_CPP17
#endif

#if __cplusplus >= 201'402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201'402L)
#  define TCB_SPAN_HAVE_CPP14
#endif

namespace rolly {
#if defined(TCB_SPAN_HAVE_CPP17) || defined(__cpp_inline_variables)
#  define TCB_SPAN_INLINE_VAR inline
#else
#  define TCB_SPAN_INLINE_VAR
#endif

#if defined(TCB_SPAN_HAVE_CPP14) || (defined(__cpp_constexpr) && __cpp_constexpr >= 201'304)
#  define TCB_SPAN_HAVE_CPP14_CONSTEXPR
#endif

#if defined(TCB_SPAN_HAVE_CPP14_CONSTEXPR)
#  define TCB_SPAN_CONSTEXPR14 constexpr
#else
#  define TCB_SPAN_CONSTEXPR14
#endif

#if defined(TCB_SPAN_HAVE_CPP14_CONSTEXPR) && (! defined(_MSC_VER) || _MSC_VER > 1'900)
#  define TCB_SPAN_CONSTEXPR_ASSIGN constexpr
#else
#  define TCB_SPAN_CONSTEXPR_ASSIGN
#endif

#if defined(TCB_SPAN_NO_CONTRACT_CHECKING)
#  define TCB_SPAN_CONSTEXPR11 constexpr
#else
#  define TCB_SPAN_CONSTEXPR11 TCB_SPAN_CONSTEXPR14
#endif

#if defined(TCB_SPAN_HAVE_CPP17) || defined(__cpp_deduction_guides)
#  define TCB_SPAN_HAVE_DEDUCTION_GUIDES
#endif

#if defined(TCB_SPAN_HAVE_CPP17) || defined(__cpp_lib_byte)
#  define TCB_SPAN_HAVE_STD_BYTE
#endif

#if defined(TCB_SPAN_HAVE_CPP17) || defined(__cpp_lib_array_constexpr)
#  define TCB_SPAN_HAVE_CONSTEXPR_STD_ARRAY_ETC
#endif

#if defined(TCB_SPAN_HAVE_CONSTEXPR_STD_ARRAY_ETC)
#  define TCB_SPAN_ARRAY_CONSTEXPR constexpr
#else
#  define TCB_SPAN_ARRAY_CONSTEXPR
#endif

#ifdef TCB_SPAN_HAVE_STD_BYTE
  using byte = std::byte;
#else
  using byte = unsigned char;
#endif

#if defined(TCB_SPAN_HAVE_CPP17)
#  define TCB_SPAN_NODISCARD [[nodiscard]]
#else
#  define TCB_SPAN_NODISCARD
#endif

  TCB_SPAN_INLINE_VAR constexpr std::size_t dynamic_extent = SIZE_MAX;

  template <typename ElementType, std::size_t Extent = dynamic_extent>
  class span;

  namespace detail {

    template <typename E, std::size_t S>
    struct span_storage {
      constexpr span_storage() noexcept = default;

      constexpr span_storage(E* p_ptr, std::size_t /*unused*/) noexcept
        : ptr(p_ptr) {}

      E* ptr = nullptr;
      static constexpr std::size_t size = S;
    };

    template <typename E>
    struct span_storage<E, dynamic_extent> {
      constexpr span_storage() noexcept = default;

      constexpr span_storage(E* p_ptr, std::size_t p_size) noexcept
        : ptr(p_ptr)
        , size(p_size) {}

      E* ptr = nullptr;
      std::size_t size = 0;
    };

// Reimplementation of C++17 std::size() and std::data()
#if defined(TCB_SPAN_HAVE_CPP17) || defined(__cpp_lib_nonmember_container_access)
    using std::data;
    using std::size;
#else
    template <class C>
    constexpr auto size(const C& c) -> decltype(c.size()) {
      return c.size();
    }

    template <class T, std::size_t N>
    constexpr std::size_t size(T const (&)[N]) noexcept {
      return N;
    }

    template <class C>
    constexpr auto data(C& c) -> decltype(c.data()) {
      return c.data();
    }

    template <class C>
    constexpr auto data(C const& c) -> decltype(c.data()) {
      return c.data();
    }

    template <class T, std::size_t N>
    constexpr T* data(T (&array)[N]) noexcept {
      return array;
    }

    template <class E>
    constexpr E const* data(std::initializer_list<E> il) noexcept {
      return il.begin();
    }
#endif  // TCB_SPAN_HAVE_CPP17

#if defined(TCB_SPAN_HAVE_CPP17) || defined(__cpp_lib_void_t)
    using std::void_t;
#else
    template <typename...>
    using void_t = void;
#endif

    template <typename T>
    using uncvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

    template <typename>
    struct is_span : std::false_type {};

    template <typename T, std::size_t S>
    struct is_span<span<T, S>> : std::true_type {};

    template <typename>
    struct is_std_array : std::false_type {};

    template <typename T, std::size_t N>
    struct is_std_array<std::array<T, N>> : std::true_type {};

    template <typename, typename = void>
    struct has_size_and_data : std::false_type {};

    template <typename T>
    struct has_size_and_data<
      T,
      void_t<decltype(detail::size(std::declval<T>())), decltype(detail::data(std::declval<T>()))>>
      : std::true_type {};

    template <typename C, typename U = uncvref_t<C>>
    struct is_container {
      static constexpr bool value = ! is_span<U>::value && ! is_std_array<U>::value
                                 && ! std::is_array<U>::value && has_size_and_data<C>::value;
    };

    template <typename T>
    using remove_pointer_t = typename std::remove_pointer<T>::type;

    template <typename, typename, typename = void>
    struct is_container_element_type_compatible : std::false_type {};

    template <typename T, typename E>
    struct is_container_element_type_compatible<
      T,
      E,
      typename std::enable_if<
        ! std::is_same<
          typename std::remove_cv<decltype(detail::data(std::declval<T>()))>::type,
          void>::value
        && std::is_convertible<
          remove_pointer_t<decltype(detail::data(std::declval<T>()))> (*)[],
          E (*)[]>::value>::type> : std::true_type {};

    template <typename, typename = size_t>
    struct is_complete : std::false_type {};

    template <typename T>
    struct is_complete<T, decltype(sizeof(T))> : std::true_type {};

  }  // namespace detail

  template <typename ElementType, std::size_t Extent>
  class span {
    static_assert(
      std::is_object<ElementType>::value,
      "A span's ElementType must be an object type (not a "
      "reference type or void)"
    );
    static_assert(
      detail::is_complete<ElementType>::value,
      "A span's ElementType must be a complete type (not a forward "
      "declaration)"
    );
    static_assert(
      ! std::is_abstract<ElementType>::value,
      "A span's ElementType cannot be an abstract class type"
    );

    using storage_type = detail::span_storage<ElementType, Extent>;

   public:
    // constants and types
    using element_type = ElementType;
    using value_type = typename std::remove_cv<ElementType>::type;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = element_type*;
    using const_pointer = element_type const*;
    using reference = element_type&;
    using const_reference = element_type const&;
    using iterator = pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;

    static constexpr size_type extent = Extent;

    // [span.cons], span constructors, copy, assignment, and destructor
    template <
      std::size_t E = Extent,
      typename std::enable_if<(E == dynamic_extent || E <= 0), int>::type = 0>
    constexpr span() noexcept {}

    TCB_SPAN_CONSTEXPR11 span(pointer ptr, size_type count)
      : storage_(ptr, count) {
      contracts::precondition(extent == dynamic_extent || count == extent);
    }

    TCB_SPAN_CONSTEXPR11 span(pointer first_elem, pointer last_elem)
      : storage_(first_elem, last_elem - first_elem) {
      contracts::precondition(
        extent == dynamic_extent || last_elem - first_elem == static_cast<std::ptrdiff_t>(extent)
      );
    }

    template <
      std::size_t N,
      std::size_t E = Extent,
      typename std::enable_if<
        (E == dynamic_extent || N == E)
          && detail::is_container_element_type_compatible<element_type (&)[N], ElementType>::value,
        int>::type = 0>
    constexpr span(element_type (&arr)[N]) noexcept
      : storage_(arr, N) {}

    template <
      typename T,
      std::size_t N,
      std::size_t E = Extent,
      typename std::enable_if<
        (E == dynamic_extent || N == E)
          && detail::is_container_element_type_compatible<std::array<T, N>&, ElementType>::value,
        int>::type = 0>
    TCB_SPAN_ARRAY_CONSTEXPR span(std::array<T, N>& arr) noexcept
      : storage_(arr.data(), N) {}

    template <
      typename T,
      std::size_t N,
      std::size_t E = Extent,
      typename std::enable_if<
        (E == dynamic_extent || N == E)
          && detail::is_container_element_type_compatible<std::array<T, N> const&, ElementType>::
            value,
        int>::type = 0>
    TCB_SPAN_ARRAY_CONSTEXPR span(std::array<T, N> const& arr) noexcept
      : storage_(arr.data(), N) {}

    template <
      typename Container,
      std::size_t E = Extent,
      typename std::enable_if<
        E == dynamic_extent && detail::is_container<Container>::value
          && detail::is_container_element_type_compatible<Container&, ElementType>::value,
        int>::type = 0>
    constexpr span(Container& cont)
      : storage_(detail::data(cont), detail::size(cont)) {}

    template <
      typename Container,
      std::size_t E = Extent,
      typename std::enable_if<
        E == dynamic_extent && detail::is_container<Container>::value
          && detail::is_container_element_type_compatible<Container const&, ElementType>::value,
        int>::type = 0>
    constexpr span(Container const& cont)
      : storage_(detail::data(cont), detail::size(cont)) {}

    constexpr span(span const& other) noexcept = default;

    template <
      typename OtherElementType,
      std::size_t OtherExtent,
      typename std::enable_if<
        (Extent == dynamic_extent || OtherExtent == dynamic_extent || Extent == OtherExtent)
          && std::is_convertible<OtherElementType (*)[], ElementType (*)[]>::value,
        int>::type = 0>
    constexpr span(span<OtherElementType, OtherExtent> const& other) noexcept
      : storage_(other.data(), other.size()) {}

    ~span() noexcept = default;

    TCB_SPAN_CONSTEXPR_ASSIGN span& operator=(span const& other) noexcept = default;

    // [span.sub], span subviews
    template <std::size_t Count>
    TCB_SPAN_CONSTEXPR11 span<element_type, Count> first() const {
      contracts::precondition(Count <= size());
      return {data(), Count};
    }

    template <std::size_t Count>
    TCB_SPAN_CONSTEXPR11 span<element_type, Count> last() const {
      contracts::precondition(Count <= size());
      return {data() + (size() - Count), Count};
    }

    template <std::size_t Offset, std::size_t Count = dynamic_extent>
    using subspan_return_t = span<
      ElementType,
      Count != dynamic_extent ? Count
                              : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent)>;

    template <std::size_t Offset, std::size_t Count = dynamic_extent>
    TCB_SPAN_CONSTEXPR11 subspan_return_t<Offset, Count> subspan() const {
      contracts::precondition(
        Offset <= size() && (Count == dynamic_extent || Offset + Count <= size())
      );
      return {data() + Offset, Count != dynamic_extent ? Count : size() - Offset};
    }

    TCB_SPAN_CONSTEXPR11 span<element_type, dynamic_extent> first(size_type count) const {
      contracts::precondition(count <= size());
      return {data(), count};
    }

    TCB_SPAN_CONSTEXPR11 span<element_type, dynamic_extent> last(size_type count) const {
      contracts::precondition(count <= size());
      return {data() + (size() - count), count};
    }

    TCB_SPAN_CONSTEXPR11 span<element_type, dynamic_extent>
      subspan(size_type offset, size_type count = dynamic_extent) const {
      contracts::precondition(
        offset <= size() && (count == dynamic_extent || offset + count <= size())
      );
      return {data() + offset, count == dynamic_extent ? size() - offset : count};
    }

    // [span.obs], span observers
    constexpr size_type size() const noexcept { return storage_.size; }

    constexpr size_type size_bytes() const noexcept { return size() * sizeof(element_type); }

    TCB_SPAN_NODISCARD constexpr bool empty() const noexcept { return size() == 0; }

    // [span.elem], span element access
    TCB_SPAN_CONSTEXPR11 reference operator[](size_type idx) const {
      contracts::precondition(idx < size());
      return *(data() + idx);
    }

    TCB_SPAN_CONSTEXPR11 reference front() const {
      contracts::precondition(! empty());
      return *data();
    }

    TCB_SPAN_CONSTEXPR11 reference back() const {
      contracts::precondition(! empty());
      return *(data() + (size() - 1));
    }

    constexpr pointer data() const noexcept { return storage_.ptr; }

    // [span.iterators], span iterator support
    constexpr iterator begin() const noexcept { return data(); }

    constexpr iterator end() const noexcept { return data() + size(); }

    TCB_SPAN_ARRAY_CONSTEXPR reverse_iterator rbegin() const noexcept {
      return reverse_iterator(end());
    }

    TCB_SPAN_ARRAY_CONSTEXPR reverse_iterator rend() const noexcept {
      return reverse_iterator(begin());
    }

   private:
    storage_type storage_ {};
  };

#ifdef TCB_SPAN_HAVE_DEDUCTION_GUIDES

  /* Deduction Guides */
  template <class T, size_t N>
  span(T (&)[N]) -> span<T, N>;

  template <class T, size_t N>
  span(std::array<T, N>&) -> span<T, N>;

  template <class T, size_t N>
  span(std::array<T, N> const&) -> span<T const, N>;

  template <class Container>
  span(Container&) -> span<
    typename std::remove_reference<decltype(*detail::data(std::declval<Container&>()))>::type>;

  template <class Container>
  span(Container const&) -> span<typename Container::value_type const>;

#endif  // TCB_HAVE_DEDUCTION_GUIDES

  template <typename ElementType, std::size_t Extent>
  constexpr span<ElementType, Extent> make_span(span<ElementType, Extent> s) noexcept {
    return s;
  }

  template <typename T, std::size_t N>
  constexpr span<T, N> make_span(T (&arr)[N]) noexcept {
    return {arr};
  }

  template <typename T, std::size_t N>
  TCB_SPAN_ARRAY_CONSTEXPR span<T, N> make_span(std::array<T, N>& arr) noexcept {
    return {arr};
  }

  template <typename T, std::size_t N>
  TCB_SPAN_ARRAY_CONSTEXPR span<T const, N> make_span(std::array<T, N> const& arr) noexcept {
    return {arr};
  }

  template <typename Container>
  constexpr span<
    typename std::remove_reference<decltype(*detail::data(std::declval<Container&>()))>::type>
    make_span(Container& cont) {
    return {cont};
  }

  template <typename Container>
  constexpr span<typename Container::value_type const> make_span(Container const& cont) {
    return {cont};
  }

  template <typename ElementType, std::size_t Extent>
  span<byte const, ((Extent == dynamic_extent) ? dynamic_extent : sizeof(ElementType) * Extent)>
    as_bytes(span<ElementType, Extent> s) noexcept {
    return {reinterpret_cast<byte const*>(s.data()), s.size_bytes()};
  }

  template <
    class ElementType,
    size_t Extent,
    typename std::enable_if<! std::is_const<ElementType>::value, int>::type = 0>
  span<byte, ((Extent == dynamic_extent) ? dynamic_extent : sizeof(ElementType) * Extent)>
    as_writable_bytes(span<ElementType, Extent> s) noexcept {
    return {reinterpret_cast<byte*>(s.data()), s.size_bytes()};
  }

  template <std::size_t N, typename E, std::size_t S>
  constexpr auto get(span<E, S> s) -> decltype(s[N]) {
    return s[N];
  }

}  // namespace rolly

namespace std {
  template <typename ElementType, size_t Extent>
  class tuple_size<rolly::span<ElementType, Extent>> : public integral_constant<size_t, Extent> {};

  template <typename ElementType>
  class tuple_size<rolly::span<ElementType, rolly::dynamic_extent>>;

  template <size_t I, typename ElementType, size_t Extent>
  class tuple_element<I, rolly::span<ElementType, Extent>> {
   public:
    static_assert(Extent != rolly::dynamic_extent && I < Extent, "");
    using type = ElementType;
  };
}  // end namespace std
