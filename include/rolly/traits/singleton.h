#pragma once

#include "pin.h"

namespace rolly {
  /**
   * @brief Singleton pattern trait.
   * @details Allows to use <b>Singleton</b> pattern in custom types and classes.
   * Example declaration:
   *
   * @code {.cpp}
   * class TestSingleton : public rolly::singleton<TestSingleton>
   * {
   *   friend struct rolly::singleton<TestSingleton>;
   *
   *  public:
   *   ~TestSingleton() = default;
   *
   *   [[nodiscard]] auto add(int a, int b) const -> int { return a + b; }
   *
   *  private:
   *   TestSingleton() = default;
   * };
   * @endcode
   *
   * Example usage:
   * @code {.cpp}
   * TestSingleton::ref().add(1, 2);
   * TestSingleton::ref_mut().add(1, 2);
   * TestSingleton::ptr()->add(1, 2);
   * TestSingleton::ptr_mut()->add(1, 2);
   * @endcode
   * @note Produced singleton is thread-safe.
   * @tparam T Class type.
   */
  template <typename T>
  struct singleton : pin {
    /**
     * @brief Returns <i>mutable pointer</i> to the singleton instance.
     * @return Mutable pointer to the singleton instance.
     * @see ptr
     */
    [[nodiscard]] static T* ptr_mut() noexcept {
      static T instance;
      return &instance;
    }

    /**
     * @brief Returns <i>pointer</i> to the singleton instance.
     * @return Pointer to the singleton instance.
     * @see ptr_mut
     */
    [[nodiscard]] static T const* ptr() noexcept { return ptr_mut(); }

    /**
     * @brief Returns <i>constant reference</i> to the singleton instance.
     * @return Constant reference to the singleton instance.
     * @see ref_mut
     */
    [[nodiscard]] static T const& ref() noexcept { return *ptr(); }

    /**
     * @brief Returns <i>mutable reference</i> to the singleton instance.
     * @return Mutable reference to the singleton instance.
     * @see ref
     */
    [[nodiscard]] static T& ref_mut() noexcept { return *ptr_mut(); }

    singleton() = default;
  };
}  // namespace rolly
