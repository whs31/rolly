#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"

#include <new>
#include <memory>
#include <iosfwd>
#include <type_traits>
#include <system_error>
#include <leaf/assert.h>
#include <leaf/traits.h>
#include <leaf/utils/noexcept.h>

namespace leaf
{
  using std::unique_ptr;
  using std::shared_ptr;
  using leaf::utils::make_unique_nothrow;
  using leaf::utils::make_shared_nothrow;

  namespace detail
  {
    template <typename T>
    concept CanBeStoredInUniquePtr = (not traits::Array<T>) and traits::Object<T>;

    template <typename T, typename = void>
    struct is_comparable_to_nullptr : std::false_type {};

    template <typename T>
    struct is_comparable_to_nullptr<
      T,
      std::enable_if_t<std::is_convertible<decltype(std::declval<T>() != nullptr), bool>::value>
    > : std::true_type {};

    /**
     * \brief Возвращает объект по константному значению или константной ссылке в зависимости от размера типа.
     */
    template <typename T>
    using value_or_reference_return_t = std::conditional_t<
      sizeof(T) < 2 * sizeof(void*) and std::is_trivially_copy_constructible_v<T>,
      T const,
      T&
    >;
  }

  /**
   * \brief Принимает сырой указатель на объект и возвращает владеющий std::unique_ptr на исходный объект.
   * \tparam T Указатель на объект. Не может быть массивом: <tt>delete T</tt> должно быть синтаксически правильным выражением.
   * \sa https://github.com/abseil/abseil-cpp/blob/master/absl/memory/memory.h
   */
  template <detail::CanBeStoredInUniquePtr T>
  [[nodiscard("do not discard result of make_unique or wrap_unique calls")]]
  auto wrap_unique(T* ptr) noexcept(noexcept(std::unique_ptr<T>(ptr))) -> std::unique_ptr<T>
  {
    return std::unique_ptr<T>(ptr);
  }

  /**
   * \brief Конвертирует <tt>std::unique_ptr</tt> в <tt>std::shared_ptr</tt>.
   * \tparam T Тип объекта, хранящегося в <tt>std::unique_ptr</tt>.
   * \tparam D Удалитель для <tt>std::unique_ptr</tt>.
   * \tparam ptr Экземпляр <tt>std::unique_ptr</tt>.
   * \return Экземпляр <tt>std::shared_ptr</tt>.
   */
  template <typename T, typename D>
  [[nodiscard("do not discard result of make_shared_from_unique call")]]
  auto make_shared_from_unique(std::unique_ptr<T, D>&& ptr) noexcept(false) -> std::shared_ptr<T>
  {
    return ptr ? std::shared_ptr<T>(std::move(ptr)) : std::shared_ptr<T>();
  }

  /**
   * \brief Запрещает указателю или умному указателю иметь нулевое значение.
   * \details Если <tt>T</tt> - указатель на объект (т.е. <tt>T == U*</tt>), то:<br>
   * - разрешено создание из <tt>U*</tt><br>
   * - запрещено создание из <tt>std::nullptr_t</tt><br>
   * - запрещен конструктор по умолчанию<br>
   * - выполняется валидация невозможности создания из нулевого <tt>U*</tt><br>
   * - разрешено неявное преобразование в <tt>U*</tt><br>
   * \note Не добавляет стоимости времени исполнения для <tt>T</tt> (<i>zero-cost abstraction</i>).
   * \tparam T
   */
  template <typename T>
  class NonNullRelaxed
  {
   public:
    static_assert(detail::is_comparable_to_nullptr<T>::value, "T must be comparable to nullptr");

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    constexpr NonNullRelaxed(U&& u) noexcept(std::is_nothrow_move_constructible_v<T>)
      : m_ptr(std::forward<U>(u))
    {
      assert_expects(this->m_ptr != nullptr);
    };

    template <typename = std::enable_if<not std::is_same_v<std::nullptr_t, T>>>
    constexpr NonNullRelaxed(T u) noexcept(std::is_nothrow_move_constructible_v<T>)
      : m_ptr(std::move(u))
    {
      assert_expects(this->m_ptr != nullptr);
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    constexpr NonNullRelaxed(NonNullRelaxed<U> const& other) noexcept(std::is_nothrow_move_constructible_v<T>)
      : NonNullRelaxed(other.get())
    {}

    NonNullRelaxed(NonNullRelaxed const& other) = default;
    NonNullRelaxed(std::nullptr_t) = delete;
    auto operator=(NonNullRelaxed const& other) -> NonNullRelaxed& = default;
    auto operator=(std::nullptr_t) -> NonNullRelaxed& = delete;
    auto operator++() -> NonNullRelaxed& = delete;
    auto operator++(int) -> NonNullRelaxed& = delete;
    auto operator--() -> NonNullRelaxed& = delete;
    auto operator--(int) -> NonNullRelaxed& = delete;
    auto operator+=(std::ptrdiff_t) -> NonNullRelaxed& = delete;
    auto operator-=(std::ptrdiff_t) -> NonNullRelaxed& = delete;
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
  auto make_non_null_relaxed(T&& t) noexcept { return NonNullRelaxed<std::remove_cv_t<std::remove_reference_t<T>>>{std::forward<T>(t)}; }

  template <typename T>
  auto operator<<(std::ostream& os, NonNullRelaxed<T> const& ptr) -> std::ostream&
  {
    os << ptr.get();
    return os;
  }

  template <typename T, class U>
  auto operator==(NonNullRelaxed<T> const& lhs, NonNullRelaxed<U> const& rhs)
  noexcept(noexcept(lhs.get() == rhs.get())) -> decltype(lhs.get() == rhs.get())
  {
    return lhs.get() == rhs.get();
  }

  template <typename T, class U>
  auto operator!=(NonNullRelaxed<T> const& lhs, NonNullRelaxed<U> const& rhs)
  noexcept(noexcept(lhs.get() != rhs.get())) -> decltype(lhs.get() != rhs.get())
  {
    return lhs.get() != rhs.get();
  }

  template <typename T, class U>
  auto operator<(NonNullRelaxed<T> const& lhs, NonNullRelaxed<U> const& rhs)
  noexcept(noexcept(std::less<>{}(lhs.get(), rhs.get()))) -> decltype(std::less<>{}(lhs.get(), rhs.get()))
  {
    return std::less<>{}(lhs.get(), rhs.get());
  }

  template <typename T, class U>
  auto operator<=(NonNullRelaxed<T> const& lhs, NonNullRelaxed<U> const& rhs)
  noexcept(noexcept(std::less_equal<>{}(lhs.get(), rhs.get()))) -> decltype(std::less_equal<>{}(lhs.get(), rhs.get()))
  {
    return std::less_equal<>{}(lhs.get(), rhs.get());
  }

  template <typename T, class U>
  auto operator>(NonNullRelaxed<T> const& lhs, NonNullRelaxed<U> const& rhs)
  noexcept(noexcept(std::greater<>{}(lhs.get(), rhs.get()))) -> decltype(std::greater<>{}(lhs.get(), rhs.get()))
  {
    return std::greater<>{}(lhs.get(), rhs.get());
  }

  template <typename T, class U>
  auto operator>=(NonNullRelaxed<T> const& lhs, NonNullRelaxed<U> const& rhs)
  noexcept(noexcept(std::greater_equal<>{}(lhs.get(), rhs.get()))) -> decltype(std::greater_equal<>{}(lhs.get(), rhs.get()))
  {
    return std::greater_equal<>{}(lhs.get(), rhs.get());
  }

  template <typename T, typename U>
  auto operator-(NonNullRelaxed<T> const& lhs, NonNullRelaxed<U> const& rhs) -> std::ptrdiff_t = delete;

  template <typename T, typename U>
  auto operator-(NonNullRelaxed<T> const& lhs, std::ptrdiff_t) -> NonNullRelaxed<U> = delete;

  template <typename T, typename U>
  auto operator+(NonNullRelaxed<T> const& lhs, NonNullRelaxed<U> const& rhs) -> std::ptrdiff_t = delete;

  template <typename T, typename U>
  auto operator+(NonNullRelaxed<T> const& lhs, std::ptrdiff_t) -> NonNullRelaxed<U> = delete;

  template <typename T, typename U = decltype(std::declval<T const&>().get()), bool = std::is_default_constructible_v<std::hash<U>>>
  struct NonNullRelaxedHash
  {
    auto operator()(NonNullRelaxed<T> const& ptr) const -> std::size_t
    {
      return std::hash<U>{}(ptr.get());
    }
  };

  template <typename T, typename U>
  struct NonNullRelaxedHash<T, U, false>
  {
    NonNullRelaxedHash() = delete;
    NonNullRelaxedHash(NonNullRelaxedHash const&) = delete;
    auto operator=(NonNullRelaxedHash const&) = delete;
  };
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-dcl58-cpp"
namespace std
{
  template <typename T>
  struct hash<leaf::NonNullRelaxed<T>> : leaf::NonNullRelaxedHash<leaf::NonNullRelaxed<T>> {};
}
#pragma clang diagnostic pop

namespace leaf
{
  template <typename T>
  class NonNull : public NonNullRelaxed<T>
  {
    
  };
}

#pragma clang diagnostic pop