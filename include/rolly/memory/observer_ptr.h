#pragma once

#include <type_traits>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include "../contracts.h"
#include "box.h"
#ifdef ___rolly_cxx20___
#  include <concepts>
#endif

// NOLINTBEGIN(*-explicit-constructor)
namespace rolly {
  /**
   * @brief A smart pointer that holds a pointer to a value, but does not own it.
   * @details observer_ptr is a non-owning pointer, or observer. The observer stores a pointer to a
   * second object, known as the watched object. An observer_ptr may also have no watched object.
   *
   * An observer is not responsible in any way for the watched object; there is no inherent
   * relationship between an observer and the object it watches.
   *
   * It is intended as a near drop-in replacement for raw pointer types, with the advantage that, as
   * a vocabulary type, it indicates its intended use without need for detailed analysis by code
   * readers.
   *
   * @remarks This is a reimplementation of the LF TSv2 <tt>std::observer_ptr</tt> type.
   * @tparam T Type of the pointer
   * @sa https://en.cppreference.com/w/cpp/experimental/observer_ptr
   * @version 2.1.31
   */
  template <typename T>
  class observer_ptr {
   public:
    /**
     * @brief The element type.
     */
    using element_type = T;

    /**
     * @brief The pointer type.
     */
    using pointer = T*;

    /**
     * @brief The const pointer type.
     */
    using const_pointer = T const*;

    /**
     * @brief The reference type.
     */
    using reference = T&;

    /**
     * @brief The const reference type.
     */
    using const_reference = T const&;

    /**
     * @brief The value type. Equivalent to the element type without cv-qualifiers.
     */
    using value_type = std::remove_cv_t<T>;

    /**
     * @brief Constructs <tt>nullptr</tt> observer.
     */
    constexpr observer_ptr() noexcept
      : ptr_(nullptr) {}

    /**
     * @brief Constructs <tt>nullptr</tt> observer.
     */
    constexpr observer_ptr(std::nullptr_t) noexcept
      : ptr_(nullptr) {}

    /**
     * @brief Constructs an observer from a pointer.
     * @param ptr Pointer to construct from.
     */
    constexpr observer_ptr(pointer ptr) noexcept
      : ptr_(ptr) {}

    /**
     * @brief Constructs an observer from another observer.
     * @tparam U Type of the other observer. Must be convertible to <tt>T*</tt>.
     * @param other Observer to construct from.
     */
#ifndef DOXYGEN_GENERATING_OUTPUT
#  ifdef ___rolly_cxx20___
    template <typename U>
      requires std::is_convertible_v<U*, T*>
#  else
    template <typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
#  endif
#else
    template <typename U>
#endif
    constexpr observer_ptr(observer_ptr<U> const& other) noexcept
      : ptr_(other.get()) {
    }

    /**
     * @brief Constructs an observer from a unique pointer.
     * @tparam U Underlying type of the unique pointer. Must be convertible to <tt>T*</tt>.
     * @param other Unique pointer to construct from.
     */
#ifndef DOXYGEN_GENERATING_OUTPUT
#  ifdef ___rolly_cxx20___
    template <typename U>
      requires std::is_convertible_v<U*, T*>
#  else
    template <typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
#  endif
#else
    template <typename U>
#endif
    constexpr observer_ptr(std::unique_ptr<U> const& other) noexcept
      : ptr_(other.get()) {
    }

    /**
     * @brief Constructs an observer from a box.
     * @tparam U Type of the box. Must be convertible to <tt>T*</tt>.
     * @param other Box to construct from.
     */
#ifndef DOXYGEN_GENERATING_OUTPUT
#  ifdef ___rolly_cxx20___
    template <typename U>
      requires std::is_convertible_v<U*, T*>
#  else
    template <typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
#  endif
#else
    template <typename U>
#endif
    constexpr observer_ptr(box<U> const& other) noexcept
      : ptr_(other.get()) {
    }

    /**
     * @brief Returns the watched pointer.
     * @return The watched pointer.
     */
    [[nodiscard]] constexpr pointer get() const noexcept { return this->ptr_; }

    /**
     * @brief Returns the constant reference to the watched object.
     * @return The reference to the watched object.
     * @invariant The watched pointer is not <tt>nullptr</tt>.
     */
    [[nodiscard]] constexpr const_reference ref() const noexcept { return **this; }

    /**
     * @brief Returns the reference to the watched object.
     * @return The reference to the watched object.
     * @invariant The watched pointer is not <tt>nullptr</tt>.
     */
    [[nodiscard]] constexpr reference ref() noexcept { return **this; }

    /**
     * @brief Returns the dereferenced watched object.
     * @return The reference to the watched object.
     * @invariant The watched pointer is not <tt>nullptr</tt>.
     */
    [[nodiscard]] constexpr reference operator*() const noexcept {
      contracts::precondition(this->ptr_ != nullptr, "Dereference of the null pointer");
      return *this->ptr_;
    }

    /**
     * @brief Returns the pointer to the watched object.
     * @return The pointer to the watched object.
     * @invariant The watched pointer is not <tt>nullptr</tt>.
     */
    [[nodiscard]] constexpr pointer operator->() const noexcept {
      contracts::precondition(this->ptr_ != nullptr, "Dereference of the null pointer");
      return this->ptr_;
    }

    /**
     * @brief Returns <tt>true</tt> if the watched pointer is not <tt>nullptr</tt>.
     */
    [[nodiscard]] explicit operator bool() const noexcept { return this->ptr_ != nullptr; }

    /**
     * @brief Returns the watched pointer.
     * @return The watched pointer.
     */
    [[nodiscard]] explicit operator pointer() const noexcept { return this->ptr_; }

    /**
     * @brief Returns the watched pointer and resets the watched pointer to <tt>nullptr</tt>.
     * @return The watched pointer.
     */
    constexpr pointer release() noexcept {
      pointer ptr = this->ptr_;
      this->ptr_ = nullptr;
      return ptr;
    }

    /**
     * @brief Resets the watched pointer to <tt>nullptr</tt> or the new watched pointer.
     * @param ptr The new watched pointer. Defaults to <tt>nullptr</tt>.
     */
    constexpr void reset(pointer ptr = nullptr) noexcept { this->ptr_ = ptr; }

    /**
     * @brief Swaps two watched pointers.
     * @param other The other observer.
     */
    constexpr void swap(observer_ptr& other) noexcept { std::swap(this->ptr_, other.ptr_); }

   private:
    pointer ptr_;
  };

  template <typename T, typename U>
  constexpr bool operator==(observer_ptr<T> const& lhs, observer_ptr<U> const& rhs) noexcept {
    return lhs.get() == rhs.get();
  }

  template <typename T, typename U>
  constexpr bool operator!=(observer_ptr<T> const& lhs, observer_ptr<U> const& rhs) noexcept {
    return lhs.get() != rhs.get();
  }

  template <typename T, typename U>
  constexpr bool operator<(observer_ptr<T> const& lhs, observer_ptr<U> const& rhs) noexcept {
    return lhs.get() < rhs.get();
  }

  template <typename T, typename U>
  constexpr bool operator>(observer_ptr<T> const& lhs, observer_ptr<U> const& rhs) noexcept {
    return lhs.get() > rhs.get();
  }

  template <typename T, typename U>
  constexpr bool operator<=(observer_ptr<T> const& lhs, observer_ptr<U> const& rhs) noexcept {
    return lhs.get() <= rhs.get();
  }

  template <typename T, typename U>
  constexpr bool operator>=(observer_ptr<T> const& lhs, observer_ptr<U> const& rhs) noexcept {
    return lhs.get() >= rhs.get();
  }

  template <typename T>
  [[nodiscard]] constexpr observer_ptr<T> make_observer(T* ptr) noexcept {
    return observer_ptr<T>(ptr);
  }
}  // namespace rolly

namespace std {
  /**
   * @brief Swaps two pointers.
   * @tparam T Type of the watched pointer
   * @param lhs First pointer
   * @param rhs Second pointer
   * @relates rolly::observer_ptr
   */
  template <typename T>
  void swap(rolly::observer_ptr<T>& lhs, rolly::observer_ptr<T>& rhs) noexcept {
    lhs.swap(rhs);
  }

  /**
   * @brief Hashes the @ref observer_ptr.
   * @tparam T Type of the watched pointer
   * @relates rolly::observer_ptr
   */
  template <typename T>
  struct hash<rolly::observer_ptr<T>> {
    std::size_t operator()(rolly::observer_ptr<T> const& ptr) const noexcept {
      return std::hash<T*> {}(ptr.get());
    }
  };
}  // namespace std

// NOLINTEND(*-explicit-constructor)
