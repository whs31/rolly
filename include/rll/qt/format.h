#pragma once

#include "../global/definitions.h"
#if defined(RLL_QT_CORE) || defined(RLL_DOC)
#  include <qstring.h>
#  include <fmt/core.h>

namespace rolly {
  namespace qt {
    template <typename C>
    [[nodiscard]] inline ::QString from_std(std::basic_string_view<C> s) {
      if constexpr(std::is_same_v<C, char>)
        return ::QString::fromUtf8(s.data(), static_cast<int>(s.size()));
      else
        return ::QString::fromUtf16(s.data(), static_cast<int>(s.size()));
    }

    template <typename C>
    [[nodiscard]] inline ::QString from_std(std::basic_string<C> const& s) {
      if constexpr(std::is_same_v<C, char>)
        return ::QString::fromUtf8(s.data(), static_cast<int>(s.size()));
      else
        return ::QString::fromUtf16(s.data(), static_cast<int>(s.size()));
    }

    template <typename C = char>
    [[nodiscard]] inline std::basic_string<C> to_std(::QString const& s) {
      if constexpr(std::is_same_v<C, char>)
        return s.toStdString();
      else
        return s.toStdWString();
    }
  }  // namespace qt

  template <typename... Args>
  [[nodiscard]] inline ::QString qformat(fmt::format_string<Args...> format, Args&&... args) {
    return qt::from_std<char>(fmt::format(format, std::forward<Args>(args)...));
  }
}  // namespace rolly
#endif
