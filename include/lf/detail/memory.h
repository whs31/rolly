#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"

#include <new>
#include <memory>
#include <iosfwd>
#include <type_traits>
#include <system_error>
#include <lf/detail/assert.h>
#include <lf/detail/traits.h>

namespace lf
{
  /// \brief Nothrow version of <tt>std::make_unique<T>()</tt>.
  template <typename T, typename... Args>
  auto make_unique(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
    -> std::unique_ptr<T>
  {
    // ReSharper disable once CppSmartPointerVsMakeFunction
    return std::unique_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
  }

  /// \brief Nothrow version of <tt>std::make_shared<T>()</tt>.
  template <typename T, typename... Args>
  auto make_shared(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
    -> std::shared_ptr<T>
  {
    // ReSharper disable once CppSmartPointerVsMakeFunction
    return std::shared_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
  }

  namespace detail
  {
    template <typename T>
    concept CanBeStoredInUniquePtr = (not trait::Array<T>) and trait::Object<T>;

    template <typename T, typename = void>
    struct is_comparable_to_nullptr : std::false_type {};

    template <typename T>
    struct is_comparable_to_nullptr<
      T,
      std::enable_if_t<std::is_convertible<decltype(std::declval<T>() != nullptr), bool>::value>
    > : std::true_type {};

    /// \brief Returns object by constant value or constant reference depending on size of type.
    template <typename T>
    using value_or_reference_return_t = std::conditional_t<
      sizeof(T) < 2 * sizeof(void*) and std::is_trivially_copy_constructible_v<T>,
      T const,
      T const&
      >;
  }

  /**
   * \brief Converts raw pointer to <tt>std::unique_ptr</tt>.
   * \tparam T Pointer type. Cannot be an array: <tt>delete T</tt> must be a valid expression.
   * \sa https://github.com/abseil/abseil-cpp/blob/master/absl/memory/memory.h
   */
  template <detail::CanBeStoredInUniquePtr T>
  [[nodiscard("do not discard result of make_unique or wrap_unique calls")]]
  auto wrap_unique(T* ptr) noexcept(noexcept(std::unique_ptr<T>(ptr))) -> std::unique_ptr<T>
  {
    return std::unique_ptr<T>(ptr);
  }

  /**
   * \brief Forbids creation of pointer type from <tt>nullptr</tt>.
   * \details If <tt>T</tt> is a pointer type (i.e. <tt>T == U*</tt>), then:<br>
   * - can be created from <tt>U*</tt><br>
   * - cannot be created from <tt>std::nullptr_t</tt><br>
   * - cannot be constructed with no-arguments<br>
   * - validates that it is possible to create from <tt>U*</tt><br>
   * - can be implicitly converted to <tt>U*</tt><br>
   * \note Does not add any overhead for <tt>T</tt> (<i>zero-cost abstraction</i>).
   * \tparam T
   */
  template <typename T>
  class NonNull
  {
   public:
    static_assert(detail::is_comparable_to_nullptr<T>::value, "T must be comparable to nullptr");

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    constexpr NonNull(U&& u) noexcept(std::is_nothrow_move_constructible_v<T>)
      : m_ptr(std::forward<U>(u))
    {
      assert_expects(this->m_ptr != nullptr);
    };

    template <typename = std::enable_if<not std::is_same_v<std::nullptr_t, T>>>
    constexpr NonNull(T u) noexcept(std::is_nothrow_move_constructible_v<T>)
      : m_ptr(std::move(u))
    {
      assert_expects(this->m_ptr != nullptr);
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    constexpr NonNull(NonNull<U> const& other) noexcept(std::is_nothrow_move_constructible_v<T>)
      : NonNull(other.get())
    {}

    NonNull(NonNull const& other) = default;
    NonNull(std::nullptr_t) = delete;
    auto operator=(NonNull const& other) -> NonNull& = default;
    auto operator=(std::nullptr_t) -> NonNull& = delete;
    auto operator++() -> NonNull& = delete;
    auto operator++(int) -> NonNull& = delete;
    auto operator--() -> NonNull& = delete;
    auto operator--(int) -> NonNull& = delete;
    auto operator+=(std::ptrdiff_t) -> NonNull& = delete;
    auto operator-=(std::ptrdiff_t) -> NonNull& = delete;
    auto operator[](std::ptrdiff_t) const -> void = delete;

    constexpr auto get() const noexcept(noexcept(detail::value_or_reference_return_t<T>(std::declval<T&>())))
      -> detail::value_or_reference_return_t<T>
    {
      return this->m_ptr;
    }

    constexpr operator T() const { return this->get(); }
    constexpr auto operator->() const -> decltype(auto) { return this->get(); }
    constexpr auto operator*() const -> decltype(auto) { return *(this->get()); }

   private:
    T m_ptr;
  };

  template <typename T>
  auto make_non_null(T&& t) noexcept { return NonNull<std::remove_cv_t<std::remove_reference_t<T>>>{std::forward<T>(t)}; }

  template <typename T>
  auto operator<<(std::ostream& os, NonNull<T> const& ptr) -> std::ostream&
  {
    os << ptr.get();
    return os;
  }

  template <typename T, class U>
  auto operator==(NonNull<T> const& lhs, NonNull<U> const& rhs)
    noexcept(noexcept(lhs.get() == rhs.get())) -> decltype(lhs.get() == rhs.get())
  {
    return lhs.get() == rhs.get();
  }

  template <typename T, class U>
  auto operator!=(NonNull<T> const& lhs, NonNull<U> const& rhs)
    noexcept(noexcept(lhs.get() != rhs.get())) -> decltype(lhs.get() != rhs.get())
  {
    return lhs.get() != rhs.get();
  }

  template <typename T, class U>
  auto operator<(NonNull<T> const& lhs, NonNull<U> const& rhs)
    noexcept(noexcept(std::less<>{}(lhs.get(), rhs.get()))) -> decltype(std::less<>{}(lhs.get(), rhs.get()))
  {
    return std::less<>{}(lhs.get(), rhs.get());
  }

  template <typename T, class U>
  auto operator<=(NonNull<T> const& lhs, NonNull<U> const& rhs)
    noexcept(noexcept(std::less_equal<>{}(lhs.get(), rhs.get()))) -> decltype(std::less_equal<>{}(lhs.get(), rhs.get()))
  {
    return std::less_equal<>{}(lhs.get(), rhs.get());
  }

  template <typename T, class U>
  auto operator>(NonNull<T> const& lhs, NonNull<U> const& rhs)
    noexcept(noexcept(std::greater<>{}(lhs.get(), rhs.get()))) -> decltype(std::greater<>{}(lhs.get(), rhs.get()))
  {
    return std::greater<>{}(lhs.get(), rhs.get());
  }

  template <typename T, class U>
  auto operator>=(NonNull<T> const& lhs, NonNull<U> const& rhs)
    noexcept(noexcept(std::greater_equal<>{}(lhs.get(), rhs.get()))) -> decltype(std::greater_equal<>{}(lhs.get(), rhs.get()))
  {
    return std::greater_equal<>{}(lhs.get(), rhs.get());
  }

  template <typename T, typename U>
  auto operator-(NonNull<T> const& lhs, NonNull<U> const& rhs) -> std::ptrdiff_t = delete;

  template <typename T, typename U>
  auto operator-(NonNull<T> const& lhs, std::ptrdiff_t) -> NonNull<U> = delete;

  template <typename T, typename U>
  auto operator+(NonNull<T> const& lhs, NonNull<U> const& rhs) -> std::ptrdiff_t = delete;

  template <typename T, typename U>
  auto operator+(NonNull<T> const& lhs, std::ptrdiff_t) -> NonNull<U> = delete;

  template <typename T, typename U = decltype(std::declval<T const&>().get()), bool = std::is_default_constructible_v<std::hash<U>>>
  struct NonNullHash
  {
    auto operator()(NonNull<T> const& ptr) const -> std::size_t
    {
      return std::hash<U>{}(ptr.get());
    }
  };

  template <typename T, typename U>
  struct NonNullHash<T, U, false>
  {
    NonNullHash() = delete;
    NonNullHash(NonNullHash const&) = delete;
    auto operator=(NonNullHash const&) = delete;
  };
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-dcl58-cpp"
namespace std
{
  template <typename T>
  struct hash<lf::NonNull<T>> : lf::NonNullHash<lf::NonNull<T>> {};
}
#pragma clang diagnostic pop
#pragma clang diagnostic pop