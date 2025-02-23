#pragma once

#include <rll/global/definitions.h>

#if defined(RLL_QT_CORE) || defined(RLL_DOC)
#  include <qstring.h>
#  include <qurl.h>

namespace rll  // NOLINT(*-concat-nested-namespaces)
{
  inline namespace literals {
    [[maybe_unused]] inline ::QString
      operator""_qstr(char16_t const* str, std::size_t size) noexcept {
#  if QT_VERSION_MAJOR < 6
      return ::QString::fromUtf16(str, static_cast<int>(size));
#  else   // QT_VERSION_MAJOR
      return ::QString(
        ::QStringPrivate(nullptr, const_cast<char16_t*>(str), static_cast<qsizetype>(size))
      );  // NOLINT(*-pro-type-const-cast)
#  endif  // QT_VERSION_MAJOR
    }

    [[maybe_unused]] inline ::QUrl
      operator""_qurl(char16_t const* str, std::size_t const size) noexcept {
      return {operator""_qstr(str, size)};
    }
  }  // namespace literals
}  // namespace rll

#endif  // RLL_QT_CORE || RLL_DOC
