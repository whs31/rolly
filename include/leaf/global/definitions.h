#pragma once

/**
 * \defgroup macros Макросы
 * \brief Макросы библиотеки Leaf
 */

#if defined(_WIN32)
#if defined(LEAF_LIBRARY)
#define LEAF_EXPORT __declspec(dllexport)
#elif defined(LEAF_STATIC_LIBRARY)
#define LEAF_EXPORT
#else
#define LEAF_EXPORT __declspec(dllimport)
#endif
#else
#define LEAF_EXPORT
#endif

// #endif
/**
 * \brief Основное пространство имен библиотеки SimKernel
 */
namespace leaf {
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
} // namespace leaf