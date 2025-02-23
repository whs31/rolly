#pragma once

#include <rll/traits/noncopyable.h>
#include <rll/traits/nonmovable.h>

namespace rll {
  /**
   * @brief Disallows copy and move operations for a type.
   * @details Useful for types that are not meant to be copied or moved.
   *
   * Example usage:
   *
   * @code {.cpp}
   *    struct my_type : rll::pin
   *    {
   *      // ...
   *    };
   *
   *    my_type t;
   *    auto t2 = t;                // will not compile
   *    auto t3 = std::move(t);     // will not compile
   * @endcode
   * @see noncopyable
   * @see nonmovable
   */
  struct pin : noncopyable,
               nonmovable {};
}  // namespace rll
