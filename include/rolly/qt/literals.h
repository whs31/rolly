#pragma once

#include "../global/definitions.h"

#if defined(ROLLY_QT_CORE) || defined(ROLLY_DOC)
#  include <qstring.h>
#  include <qurl.h>

namespace rolly  // NOLINT(*-concat-nested-namespaces)
{
  inline namespace literals {
    [[maybe_unused]] inline ::QString operator""_qs(char16_t const* str, std::size_t size) noexcept {
#  if QT_VERSION_MAJOR < 6
      return ::QString::fromUtf16(str, static_cast<int>(size));
#  else   // QT_VERSION_MAJOR
      return ::QString(::QStringPrivate(nullptr, const_cast<char16_t*>(str), static_cast<qsizetype>(size))
      );  // NOLINT(*-pro-type-const-cast)
#  endif  // QT_VERSION_MAJOR
    }

    [[maybe_unused]] inline ::QUrl operator""_qurl(char16_t const* str, std::size_t size) noexcept {
      return {operator""_qs(str, size)};
    }
  }  // namespace literals
}  // namespace rolly

#endif  // ROLLY_QT_CORE || ROLLY_DOC
