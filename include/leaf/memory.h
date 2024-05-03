#pragma once

#include <new>
#include <memory>
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
    static_assert(detail::is_comparable_to_nullptr<T>::value, "T must be comparable to nullptr");
  };
}