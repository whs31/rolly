#pragma once

#include "../global/definitions.h"

#if defined(ROLLY_QT_CORE) || defined(ROLLY_DOC)
# include <qstring.h>
# include <qpoint.h>
# include <fmt/core.h>
# include <fmt/format.h>

namespace rolly::qt {} // namespace rolly::qt
namespace fmt
{
  template <>
  struct formatter<QString> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    static auto format(const QString& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", s.toStdString());
    }
  };

  template <>
  struct formatter<QByteArray> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    static auto format(const QByteArray& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", s.toStdString());
    }
  };

  template <>
  struct formatter<QLatin1String> {
    template<typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    static auto format(const QLatin1String& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", std::string(s.data()));
    }
  };

  template <>
  struct formatter<QStringView> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    static auto format(const QStringView& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", s.toString().toStdString());
    }
  };

  template <>
  struct formatter<QChar> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    static auto format(const QChar& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", s.unicode());
    }
  };

  template <>
  struct formatter<QPoint> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    static auto format(const QPoint& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "[{}, {}]", s.x(), s.y());
    }
  };

  template <>
  struct formatter<QPointF> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    static auto format(const QPointF& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "[{}, {}]", s.x(), s.y());
    }
  };
} // namespace fmt
#endif // ROLLY_QT_CORE || ROLLY_DOC