#pragma once

/**
 * \defgroup macros Макросы
 * \brief Макросы библиотеки SimKernel
 */

/**
 * \ingroup macros
 * \brief Помечает переменную как неиспользуемую
 * \details Убирает предупреждения компиляторов и статических анализаторов
 * о неиспользуемой переменной.
 * \param var Переменная
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
 */
#define $unimplemented() \
  throw std::runtime_error("unimplemented code");

/**
 * \brief Основное пространство имен библиотеки SimKernel
 */
namespace simkernel {}