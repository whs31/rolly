#pragma once

/**
 * \defgroup macros Макросы
 * \brief Макросы библиотеки Leaf
 */

//#ifdef DOXYGEN
/**
 * \ingroup macros
 * \brief Помечает переменную как неиспользуемую
 * \details Убирает предупреждения компиляторов и статических анализаторов
 * о неиспользуемой переменной.
 * \param var Переменная
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь используется аттрибут
 * `[[maybe_unused]]`.
 */
#define $unused(var) \
  (void)(var)

/**
 * \ingroup macros
 * \brief Помечает код как недостижимый
 * \details Отмечает код как недостижимый для компилятора. Необходимо для
 * предотвращения предупреждений компилятора/анализаторов о том, что не все
 * пути выполнения функции возвращают значение.
 *
 * Пример использования:
 * ```cpp
 * auto loop() -> int
 * {
 *    while(true)
 *    {
 *      if(cond)
 *        return 0;
 *    }
 *    $unreachable()
 * }
 * ```
 * Если код, отмеченный таким образом, все же будет выполнен, то будет
 * вызвано исключение.
 * \throw std::runtime_error
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь используется встроенная
 * стандартная функция `std::unreachable` (**с С++23** и выше), либо ее полный аналог `leaf::unreachable`.
 * \see leaf::unreachable
 */
#define $unreachable() \
  throw std::runtime_error("unreachable code reached");

/**
 * \ingroup macros
 * \brief Помечает участок кода как не имеющий реализации
 * \details Макрос удобен для замены комментариев с `todo`.
 * Если код, отмеченный таким образом, все же будет выполнен, то будет
 * вызвано исключение.
 * \throw std::runtime_error
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь необходимо явно написать
 * выброс исключения, или любой другой способ завершения программы/вызова ошибки.
 */
#define $unimplemented() \
  throw std::runtime_error("unimplemented code");

//#endif
/**
 * \brief Основное пространство имен библиотеки SimKernel
 */
namespace leaf
{
  /**
   * \brief Вызывает неопределенное поведение.
   * \details https://en.cppreference.com/w/cpp/utility/unreachable
   */
  [[noreturn]] inline auto unreachable() -> void
  {
    #if defined(_MSC_VER) && !defined(__clang__) // MSVC
    __assume(false);
    #else // GCC, Clang
    __builtin_unreachable();
    #endif
  }
}