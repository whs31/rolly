#pragma once

#include <memory>
#include <rll/memory/propagate_const.h>

namespace rll {
  /**
   * @brief Pointer-to-implementation pattern trait.
   * @details Allows to use <b>Pimpl</b> pattern in custom types and classes.
   *
   * Example declaration:
   *
   * @code {.cpp}
   *    class TestPimpl : rll::pin
   *    {
   *      public:
   *         TestPimpl() = default;
   *         [[nodiscard]] auto add(int a, int b) const -> int;
   *
   *      private:
   *        struct impl;
   *        rll::pimpl<struct impl> impl_;
   *    };
   *
   *    struct TestPimpl::impl
   *    {
   *        int add(int a, int b) const { return a + b; }
   *    };
   *
   *    int TestPimpl::add(int a, int b) const {
   *      return this->impl_->add(a, b);
   *    }
   * @endcode
   * @tparam T Type to implement.
   */
  template <typename T>
  using pimpl = propagate_const<std::unique_ptr<T>>;

  template <typename T>
  using shared_pimpl = propagate_const<std::shared_ptr<T>>;
}  // namespace rll

#define DECLARE_PRIVATE(classname)                                                            \
  struct classname## Private;                                                                 \
  rll::pimpl<classname## Private> impl;

#define DECLARE_PRIVATE_AS(alias)                                                             \
  struct alias;                                                                               \
  rll::pimpl<alias> impl;

#define DECLARE_PRIVATE_SHARED(classname)                                                     \
  struct classname## Private;                                                                 \
  rll::shared_pimpl<classname## Private> impl;

#define DECLARE_PRIVATE_SHARED_AS(alias)                                                      \
  struct alias;                                                                               \
  rll::shared_pimpl<alias> impl;
