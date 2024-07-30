#pragma once

#include <vector>
#include <span>

namespace floppy
{
  /// \brief Alias for <tt>std::vector<std::byte></tt>.
  /// \details Provides owning byte array.
  /// \ingroup aliases
  /// \headerfile floppy/bytearray.h
  /// \see bytearray_view, bytearray_view_mut
  using bytearray = std::vector<std::byte>;

  /// \brief Alias for <tt>std::span<std::byte const></tt>.
  /// \details Provides non-owning view over a byte array.
  /// \ingroup aliases
  /// \headerfile floppy/bytearray.h
  /// \see bytearray, bytearray_view_mut
  using bytearray_view = std::span<std::byte const>;

  /// \brief Alias for <tt>std::span<std::byte></tt>.
  /// \details Provides mutable non-owning view over a byte array.
  /// \ingroup aliases
  /// \headerfile floppy/bytearray.h
  /// \see bytearray, bytearray_view
  using bytearray_view_mut = std::span<std::byte>;
} // namespace floppy