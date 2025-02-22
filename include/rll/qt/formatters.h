#pragma once

#include "../global/definitions.h"

#if defined(RLL_QT_CORE) || defined(RLL_DOC)
#  include <qstring.h>
#  include <qpoint.h>
#  include <fmt/core.h>
#  include <fmt/format.h>

namespace rolly {
  /**
   * @brief Qt-related types and functions.
   * @note Only available if Qt::Core is linked against the project.
   */
  namespace qt {}  // namespace qt
}  // namespace rolly

namespace fmt {
  /**
   * @brief Specialization of the <code>fmt::formatter</code> for the <code>QString</code> class.
   * @note Only available if Qt::Core is linked against the project.
   */
  template <>
  struct formatter<QString> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <typename FormatContext>
    static auto format(QString const& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", s.toStdString());
    }
  };

  /**
   * @brief Specialization of the <code>fmt::formatter</code> for the <code>QByteArray</code> class.
   * @note Only available if Qt::Core is linked against the project.
   */
  template <>
  struct formatter<QByteArray> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <typename FormatContext>
    static auto format(QByteArray const& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", s.toStdString());
    }
  };

  /**
   * @brief Specialization of the <code>fmt::formatter</code> for the <code>QLatin1String</code>
   * class.
   * @note Only available if Qt::Core is linked against the project.
   */
  template <>
  struct formatter<QLatin1String> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <typename FormatContext>
    static auto format(QLatin1String const& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", std::string(s.data()));
    }
  };

  /**
   * @brief Specialization of the <code>fmt::formatter</code> for the <code>QStringView</code>
   * class.
   * @note Only available if Qt::Core is linked against the project.
   */
  template <>
  struct formatter<QStringView> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <typename FormatContext>
    static auto format(QStringView const& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", s.toString().toStdString());
    }
  };

  /**
   * @brief Specialization of the <code>fmt::formatter</code> for the <code>QChar</code> class.
   * @note Only available if Qt::Core is linked against the project.
   */
  template <>
  struct formatter<QChar> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <typename FormatContext>
    static auto format(QChar const& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", s.unicode());
    }
  };

  /**
   * @brief Specialization of the <code>fmt::formatter</code> for the <code>QPoint</code> class.
   * @note Only available if Qt::Core is linked against the project.
   */
  template <>
  struct formatter<QPoint> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <typename FormatContext>
    static auto format(QPoint const& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "[{}, {}]", s.x(), s.y());
    }
  };

  /**
   * @brief Specialization of the <code>fmt::formatter</code> for the <code>QPointF</code> class.
   * @note Only available if Qt::Core is linked against the project.
   */
  template <>
  struct formatter<QPointF> {
    template <typename ParseContext>
    constexpr static auto parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <typename FormatContext>
    static auto format(QPointF const& s, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "[{}, {}]", s.x(), s.y());
    }
  };
}  // namespace fmt
#endif  // RLL_QT_CORE || RLL_DOC
