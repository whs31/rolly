#pragma once

namespace rolly
{
  /**
   * @brief Disallows move operations for a type
   * @details Example usage:
   *
   * @code {.cpp}
   *    class Test : rolly::nonmovable {}; // move is not allowed for Test
   *
   *    Test t1;
   *    Test t2(std::move(t1)); // error
   * @endcode
   * @see noncopyable
   * @see pin
   */
  struct nonmovable // NOLINT(*-special-member-functions)
  {
    nonmovable() = default;
    ~nonmovable() = default;
    nonmovable(nonmovable&&) noexcept = delete;
    nonmovable& operator=(nonmovable&&) noexcept = delete;
  };
} // namespace rolly