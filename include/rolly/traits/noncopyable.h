#pragma once

namespace rolly
{
  /**
   * @brief Disallows copy operations for a type
   * @details Example usage:
   *
   * @code {.cpp}
   *    class Test : rolly::noncopyable {}; // copy is not allowed for Test
   *
   *    Test t1;
   *    Test t2(t1); // error
   * @endcode
   * @see nonmovable
   * @see pin
   */
  struct noncopyable // NOLINT(*-special-member-functions)
  {
    noncopyable() = default;
    ~noncopyable() = default;
    noncopyable(noncopyable const&) = delete;
    noncopyable& operator=(noncopyable const&) = delete;
  };
} // namespace rolly