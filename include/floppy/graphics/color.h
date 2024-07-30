/// \file floppy/graphics/color.h
/// \brief Class representing a color.
/// \author whs31

#pragma once

#include <array>
#include <floppy/detail/math.h>
#include <floppy/detail/formatters.h>
#include <floppy/traits.h>

#if defined(FL_QT_GUI) || defined(FL_DOC)
# include <qcolor.h>
#endif


/// \brief Namespace with types related to graphics and drawing.
namespace floppy::gfx
{
  /// \brief Implementation details for classes and functions related to \ref gfx.
  namespace detail
  {
    [[nodiscard]] constexpr auto hex_to_dec(char const c) -> i32 {
      if (c >= '0' and c <= '9')
        return c - '0';
      if (c >= 'a' and c <= 'f') 
        return c - 'a' + 10;
      if (c >= 'A' and c <= 'F') 
        return c - 'A' + 10;
      throw std::invalid_argument("invalid hex character");
    }

    template <typename T> constexpr auto min(T&& v) -> T { return std::forward<T>(v); }
    template <typename T, typename... Args>
    constexpr auto min(T const& v1, T const& v2, Args const&... args) -> T {
      return v2 < v1 ? min(v2, args...) : min(v1, args...);
    }

    template <typename T>
    [[maybe_unused]] constexpr auto max(T&& v) -> T { return std::forward<T>(v); }

    template <typename T, typename... Args>
    constexpr auto max(T const& v1, T const& v2, Args const&... args) -> T {
      return v2 > v1 ? max(v2, args...) : max(v1, args...);
    }

    template <typename T> constexpr auto abs(T const& v) -> T { return v < T(0) ? -v : v; }
    constexpr auto fmod(f32 const x, f32 const y) -> f32 { return x - y * std::floor(x / y); }

    /// \brief Converts a hex string to an RGB color.
    /// \param str Hex string in RGB format as <i>C-string</i>.
    /// \return RGB color as <code>std::array<u8, 3></code>.
    /// \note Assumes the string is 6 characters long.
    constexpr auto str_to_rgb(char const* str) -> std::array<u8, 3> {
      return {
        static_cast<u8>(hex_to_dec(str[0]) << 4 | hex_to_dec(str[1])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[2]) << 4 | hex_to_dec(str[3])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[4]) << 4 | hex_to_dec(str[5]))  // NOLINT(*-pro-bounds-pointer-arithmetic)
      };
    }

    /// \brief Converts a hex string to an RGBA color.
    /// \param str Hex string in RGBA format as <i>C-string</i>.
    /// \return RGBA color as <code>std::array<u8, 4></code>.
    /// \note Assumes the string is 8 characters long.
    constexpr auto str_to_rgba(char const* str) -> std::array<u8, 4> {
      return {
        static_cast<u8>(hex_to_dec(str[0]) << 4 | hex_to_dec(str[1])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[2]) << 4 | hex_to_dec(str[3])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[4]) << 4 | hex_to_dec(str[5])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[6]) << 4 | hex_to_dec(str[7]))  // NOLINT(*-pro-bounds-pointer-arithmetic)
      };
    }

    namespace hsl
    {
      [[nodiscard]] constexpr auto k(i32 const n, u16 const hue) -> f32 {
        return fmod(static_cast<f32>(n) + static_cast<f32>(hue) / 30.0F, 12.0F);
      }

      [[nodiscard]] constexpr auto a(f32 const saturation, f32 const lightness) -> f32 {
        return saturation * min(lightness, 1.0F - lightness);
      }

      [[nodiscard]] constexpr auto chroma(std::array<u8, 3> const& rgb) -> f32 {
        return static_cast<f32>(max(rgb[0], rgb[1], rgb[2]) - min(rgb[0], rgb[1], rgb[2])) / 255.0F;
      }

      [[nodiscard]] constexpr auto h(std::array<u8, 3> const& rgb) -> f32 {
        return chroma(rgb) == 0.0F
          ? 0.0F
          : (
            max(rgb[0], rgb[1], rgb[2]) == rgb[0]
              ? static_cast<f32>(rgb[1] - rgb[2]) / 255.0F / chroma(rgb) + (rgb[1] < rgb[2]
                ? 6.0F
                : 0.0F)
              : (max(rgb[0], rgb[1], rgb[2]) == rgb[1]
                ? static_cast<f32>(rgb[2] - rgb[0]) / 255.0F / chroma(rgb) + 2.0F
                : (static_cast<f32>(rgb[0] - rgb[1]) / 255.0F / chroma(rgb) + 4.0F))
          );
      }

      [[nodiscard]] constexpr auto lightness(std::array<u8, 3> const& rgb) -> f32 {
        return static_cast<f32>(min(rgb[0], rgb[1], rgb[2]) + max(rgb[0], rgb[1], rgb[2])) / 510.0F;
      }

      [[nodiscard]] constexpr auto hue(f32 const h) -> u16 {
        return static_cast<u16>(std::round(h * 60.0F) < 0.0F
          ? std::round(h * 60.0F) + 360.0F
          : std::round(h * 60.0F));
      }
    } // namespace hsl
  } // namespace detail

  /// \brief Class representing a color.
  /// \details The color type is used by the graphics module to represent colors. Provides useful
  /// functions to convert between different color formats.
  /// \headerfile floppy/graphics.h
  /// \ingroup gfx
  class color : public traits::formattable<color, char>
  {
    template <concepts::num T = u8>
    static constexpr floppy::concepts::num auto mask = static_cast<T>(std::numeric_limits<u8>::max());

   public:
    /// \brief HSL color representation.
    /// \headerfile floppy/graphics.h
    /// \sa https://en.wikipedia.org/wiki/HSL_and_HSV
    struct hsl_t : public traits::formattable<color::hsl_t, char>
    {
      u16 h = 0;     ///< Hue
      f32 s = 0.0F;  ///< Saturation
      f32 l = 0.0F;  ///< Lightness

      /// \brief Constructs an HSL color.
      /// \param h Hue
      /// \param s Saturation
      /// \param l Lightness
      constexpr hsl_t(u16 h, f32 s, f32 l)
        : h(h)
        , s(s)
        , l(l)
      {}

      /// \brief Converts the color to RGB format.
      [[nodiscard]] constexpr auto to_rgb() const -> std::array<u8, 3> {
        return std::array<u8, 3> {
          static_cast<u8>((this->l - detail::hsl::a(this->s, this->l)
            * detail::max(-1.0F, detail::min(detail::hsl::k(0, this->h) - 3.0F,
            9.0F - detail::hsl::k(0, this->h), 1.0F))) * 255.0F),
          static_cast<u8>((this->l - detail::hsl::a(this->s, this->l)
            * detail::max(-1.0F, detail::min(detail::hsl::k(8, this->h) - 3.0F,
            9.0F - detail::hsl::k(8, this->h), 1.0F))) * 255.0F),
          static_cast<u8>((this->l - detail::hsl::a(this->s, this->l)
            * detail::max(-1.0F, detail::min(detail::hsl::k(4, this->h) - 3.0F,
            9.0F - detail::hsl::k(4, this->h), 1.0F))) * 255.0F)
        };
      }

      /// \brief Constructs an HSL color from RGB values.
      /// \param r Red
      /// \param g Green
      /// \param b Blue
      [[nodiscard]] static constexpr auto from_rgb(u8 const r, u8 const g, u8 const b) -> hsl_t {
        return hsl_t {
          detail::hsl::hue(detail::hsl::h(std::array<u8, 3>{r, g, b})),
          detail::hsl::chroma(std::array<u8, 3>{r, g, b}) == 0.0F
            ? 0.0F
            : detail::hsl::chroma(std::array<u8, 3>{r, g, b}) / (1 - detail::abs(2 * detail::hsl::lightness(std::array<u8, 3>{r, g, b}) - 1)),
          detail::hsl::lightness(std::array<u8, 3>{r, g, b})
        };
      }

      /// \brief Converts the color to a string.
      /// \details Example: <code>(180 0.5 0.5)</code>.
      /// \returns The string representation of the color.
      [[nodiscard]] virtual auto to_string() const -> std::string override {
        return fmt::format("({} {} {})", this->h, this->s, this->l);
      }

      constexpr auto operator==(color::hsl_t const& other) const -> bool {
        return (this->h == other.h)
          and math::approx_eq(this->s, other.s, 3.0F)
          and math::approx_eq(this->l, other.l, 3.0F);
      }

      constexpr auto operator!=(color::hsl_t const& other) const -> bool { return not (*this == other); }
    };

    /// \brief HSLA color representation.
    /// \headerfile floppy/graphics.h
    /// \sa https://en.wikipedia.org/wiki/HSL_and_HSV
    struct hsla_t : public traits::formattable<color::hsla_t, char>
    {
      u16 h = 0;     ///< Hue
      f32 s = 0.0F;  ///< Saturation
      f32 l = 0.0F;  ///< Lightness
      f32 a = 1.0F;  ///< Alpha

      /// \brief Constructs an HSLA color.
      /// \param h Hue
      /// \param s Saturation
      /// \param l Lightness
      /// \param a Alpha
      constexpr hsla_t(const u16 h, const f32 s, const f32 l, const f32 a)
        : h(h)
        , s(s)
        , l(l)
        , a(a)
      {}

      /// \brief Converts the color to RGBA format.
      [[nodiscard]] constexpr auto to_rgba() const -> std::array<u8, 4> {
        const auto rgb = color::hsl_t { this->h, this->s, this->l }.to_rgb();
        return std::array<u8, 4> {
          static_cast<u8>(rgb[0]),
          static_cast<u8>(rgb[1]),
          static_cast<u8>(rgb[2]),
          static_cast<u8>(this->a * 255.0F)
        };
      }

      /// \brief Converts the color to a string.
      /// \details Example: <code>(180 0.5 0.5 0.5)</code>.
      /// \returns The string representation of the color.
      [[nodiscard]] virtual auto to_string() const -> std::string override {
        return fmt::format("({} {} {} {})", this->h, this->s, this->l, this->a);
      }

      constexpr auto operator==(color::hsla_t const& other) const -> bool {
        return (this->h == other.h)
          and math::approx_eq(this->s, other.s, 3.0F)
          and math::approx_eq(this->l, other.l, 3.0F)
          and math::approx_eq(this->a, other.a, 3.0F);
      }

      constexpr auto operator!=(color::hsla_t const& other) const -> bool { return not (*this == other); }
    };

    /// \brief HSV color representation.
    /// \headerfile floppy/graphics.h
    /// \sa https://en.wikipedia.org/wiki/HSL_and_HSV
    struct hsv_t : public traits::formattable<color::hsv_t, char>
    {
      u16 h = 0.0F;
      f32 s = 0.0F;
      f32 v = 0.0F;

      /// \brief Constructs an HSL color.
      /// \param h Hue
      /// \param s Saturation
      /// \param v Value
      constexpr hsv_t(const u16 h, const f32 s, const f32 v)
        : h(h)
        , s(s)
        , v(v)
      {}

      /// \brief Converts the color to RGB format.
      [[nodiscard]] constexpr auto to_rgb() const -> std::array<u8, 3> {
        auto rgb = std::array<u8, 3>();
        auto tmp_rgb  = std::array<f32, 3>();
        const auto c = this->v * this->s ;
        const auto x = c * (1.0F - std::abs(std::fmod(static_cast<f32>(this->h) / 60.0F, 2.0F) - 1.0F));
        const auto m = this->v - c;
        if(static_cast<f32>(this->h)< 60.F)
          tmp_rgb = {c, x ,0};
        else if(static_cast<f32>(this->h) < 120.F)
          tmp_rgb = {x, c, 0};
        else if(static_cast<f32>(this->h) < 180.F)
          tmp_rgb = {0, c, x};
        else if(static_cast<f32>(this->h) < 240.F)
          tmp_rgb = {0, x, c};
        else if(static_cast<f32>(this->h) < 300.F)
          tmp_rgb = {x, 0, c};
        else if(static_cast<f32>(this->h)< 360.F)
          tmp_rgb = {c, 0, x};

        rgb[0] = static_cast<u8>((tmp_rgb[0] + m) * 255.0F);
        rgb[1] = static_cast<u8>((tmp_rgb[1] + m) * 255.0F);
        rgb[2] = static_cast<u8>((tmp_rgb[2] + m) * 255.0F);
        return rgb;
      }

      /// \brief Constructs an HSL color from RGB values.
      /// \param r Red
      /// \param g Green
      /// \param b Blue
      [[nodiscard]] static constexpr auto from_rgb(u8 const r, u8 const g, u8 const b) -> hsv_t {
        auto self = hsv_t(0, 0, 0);
        auto const fr = static_cast<f32>(r) / 255.0F;
        auto const fg = static_cast<f32>(g) / 255.0F;
        auto const fb = static_cast<f32>(b) / 255.0F;
        auto const c_max = detail::max(detail::max(fr, fg), fb);
        auto const c_min = detail::min(detail::min(fr, fg), fb);
        auto const delta = c_max - c_min;

        if (delta <= 0.F)
          self.h = 0.F;
        else {
          if(c_max == fr)
            self.h = std::lround(60.0F * std::fmod((fg - fb) / delta, 6.0F));
          else if(c_max == fg)
            self.h = std::lround(60.0F * ((fb - fr) / delta + 2.F));
          else if(c_max == fb)
            self.h = std::lround(60.0 * ((fr - fg) / delta + 4.F));
        }

        self.s = c_max == 0.F ? 0.F : delta / c_max;
        self.v = c_max;

        return self;
      }

      /// \brief Converts the color to a string.
      /// \details Example: <code>(180 0.5 0.5)</code>.
      /// \returns The string representation of the color.
      [[nodiscard]] virtual auto to_string() const -> std::string override {
        return fmt::format("({} {} {})", this->h, this->s, this->v);
      }

      constexpr auto operator==(color::hsv_t const& other) const -> bool {
        return (this->h == other.h)
          and math::approx_eq(this->s, other.s, 3.0F)
          and math::approx_eq(this->v, other.v, 3.0F);
      }

      constexpr auto operator!=(color::hsv_t const& other) const -> bool { return not (*this == other); }
    };

    /// \brief HSVA color representation.
    /// \headerfile floppy/graphics.h
    /// \sa https://en.wikipedia.org/wiki/HSL_and_HSV
    struct hsva_t : public traits::formattable<color::hsva_t, char>
    {
      u16 h = 0.0F;
      f32 s = 0.0F;
      f32 v = 0.0F;
      f32 a = 1.0F;

      /// \brief Constructs an HSVA color.
      /// \param h Hue
      /// \param s Saturation
      /// \param v Value
      /// \param a Alpha
      constexpr hsva_t(const u16 h, const f32 s, const f32 v, const f32 a)
        : h(h)
        , s(s)
        , v(v)
        , a(a)
      {}

      /// \brief Converts the color to RGBA format.
      [[nodiscard]] constexpr auto to_rgba() const -> std::array<u8, 4> {
        const auto rgb = color::hsv_t { this->h, this->s, this->v }.to_rgb();
        return std::array<u8, 4> {
          static_cast<u8>(rgb[0]),
          static_cast<u8>(rgb[1]),
          static_cast<u8>(rgb[2]),
          static_cast<u8>(this->a * 255.0F)
        };
      }

      /// \brief Converts the color to a string.
      /// \details Example: <code>(180 0.5 0.5 0.5)</code>.
      /// \returns The string representation of the color.
      [[nodiscard]] virtual auto to_string() const -> std::string override {
        return fmt::format("({} {} {} {})", this->h, this->s, this->v, this->a);
      }

      constexpr auto operator==(color::hsva_t const& other) const -> bool {
        return (this->h == other.h)
          and math::approx_eq(this->s, other.s, 3.0F)
          and math::approx_eq(this->v, other.v, 3.0F)
          and math::approx_eq(this->a, other.a, 3.0F);
      }

      constexpr auto operator!=(color::hsva_t const& other) const -> bool { return not (*this == other); }
    };

    struct cmyk_t : public traits::formattable<color::hsv_t, char>
    {
      f32 c = 0.0F;
      f32 m = 0.0F;
      f32 y = 0.0F;
      f32 k = 0.0F;

      constexpr cmyk_t(const f32 c, const f32 m, const f32 y, const f32 k)
        : c{c}
        , m{m}
        , y{y}
        , k{k}
      {}

      [[nodiscard]] constexpr auto to_rgb() const -> std::array<u8, 3> {
        return {
          static_cast<u8>(255.0F * (1.0F - this->c) * (1.0F - this->k)),
          static_cast<u8>(255.0F * (1.0F - this->m) * (1.0F - this->k)),
          static_cast<u8>(255.0F * (1.0F - this->y) * (1.0F - this->k))
        };
      }

      [[nodiscard]] static constexpr auto from_rgb(const u8 r, const u8 g, const u8 b) -> color::cmyk_t {
        const auto tmp_rgb = std::array<f32, 3>{
          static_cast<f32>(r) / 255.0F,
          static_cast<f32>(g) / 255.0F,
          static_cast<f32>(b) / 255.0F
        };
        const auto k = 1 - std::max(std::max(tmp_rgb[0], tmp_rgb[1]), tmp_rgb[2]);
        const auto c = (1 - tmp_rgb[0] - k) / (1 - k);
        const auto m = (1 - tmp_rgb[1] - k) / (1 - k);
        const auto y = (1 - tmp_rgb[2] - k) / (1 - k);
        return {c, m, y, k};
      }

      [[nodiscard]] virtual auto to_string() const -> std::string override {
        return fmt::format("({} {} {} {})", this->c, this->m, this->y, this->k);
      }

      constexpr auto operator==(color::cmyk_t const& other) const -> bool {
        return math::approx_eq(this->c, other.c, 3.0F)
          and math::approx_eq(this->m, other.m, 3.0F)
          and math::approx_eq(this->y, other.y, 3.0F)
          and math::approx_eq(this->k, other.k, 3.0F);
      }


      constexpr auto operator!=(color::cmyk_t const& other) const -> bool
      {
        return not (*this == other);
      }
    };

    struct cmyka_t : public traits::formattable<color::hsv_t, char>
    {
      f32 c = 0.0F;
      f32 m = 0.0F;
      f32 y = 0.0F;
      f32 k = 0.0F;
      f32 a = 1.0F;

      constexpr cmyka_t(const f32 c, const f32 m, const f32 y, const f32 k, const f32 a)
        : c{c}
        , m{m}
        , y{y}
        , k{k}
        , a{a}
      {}

      [[nodiscard]] constexpr auto to_rgba() const -> std::array<u8, 4> {
        return {
          static_cast<u8>(255.0F * (1.0F - this->c) * (1.0F - this->k)),
          static_cast<u8>(255.0F * (1.0F - this->m) * (1.0F - this->k)),
          static_cast<u8>(255.0F * (1.0F - this->y) * (1.0F - this->k)),
          static_cast<u8>(255.0F * this->a)
        };
      }

      [[nodiscard]] static constexpr auto from_rgba(const u8 r, const u8 g, const u8 b, const u8 a) -> color::cmyka_t
      {
        auto tmp_cmyk = cmyk_t::from_rgb(r, g, b);
        return {
          tmp_cmyk.c,
          tmp_cmyk.m,
          tmp_cmyk.y,
          tmp_cmyk.k,
          static_cast<f32>(a)/255.F};
      }

      [[nodiscard]] virtual auto to_string() const -> std::string override {
        return fmt::format("({} {} {} {} {})", this->c, this->m, this->y, this->k, this->a);
      }

      constexpr auto operator==(color::cmyka_t const& other) const -> bool {
        return math::approx_eq(this->c, other.c, 3.0F)
          and math::approx_eq(this->m, other.m, 3.0F)
          and math::approx_eq(this->y, other.y, 3.0F)
          and math::approx_eq(this->k, other.k, 3.0F)
          and math::approx_eq(this->a, other.a, 3.0F);
      }

      constexpr auto operator!=(color::cmyka_t const& other) const -> bool { return not (*this == other); }
    };

    /// \brief Constructs a default black opaque color.
    constexpr color()
      : r_(0.0F)
      , g_(0.0F)
      , b_(0.0F)
      , a_(1.0F)
    {}

    /// \brief Constructs an opaque color from rgb values.
    /// \param r Red component.
    /// \param g Green component.
    /// \param b Blue component.
    constexpr color(const u8 r, const u8 g, const u8 b)
      : r_(static_cast<f32>(r) / mask<f32>)
      , g_(static_cast<f32>(g) / mask<f32>)
      , b_(static_cast<f32>(b) / mask<f32>)
      , a_(1.0F)
    {}

    /// \brief Constructs a color from rgba values.
    /// \param r Red component.
    /// \param g Green component.
    /// \param b Blue component.
    /// \param a Alpha component.
    constexpr color(const u8 r, const u8 g, const u8 b, const u8 a)
      : r_(static_cast<f32>(r) / mask<f32>)
      , g_(static_cast<f32>(g) / mask<f32>)
      , b_(static_cast<f32>(b) / mask<f32>)
      , a_(static_cast<f32>(a) / mask<f32>)
    {}

    /// \brief Constructs a color from 32-bit unsigned integer.
    /// \details The upper 8 bits are the red component,
    /// the next 8 bits are the green component, the next 8 bits are the blue component, and the
    /// last 8 bits are the alpha component.
    /// For example, the color <b>#32a852</b> is represented as <code>0x32a852FF</code> and
    /// color <b>#10101010</b> is represented as <code>0x10101010</code>.
    /// \note If passed integer is greater than 0xFFFFFF, the lower 24 bits are used as red component,
    /// the next 8 bits are used as green component, the next 8 bits are used as blue component, and
    /// the last 8 bits are used as alpha component.
    /// \param rgba 32-bit unsigned integer.
    constexpr explicit color(u32 rgba) {
      if(rgba > 0xFFFFFF) {
        this->r_ = static_cast<f32>((rgba >> 24) & mask<u8>) / mask<f32>;
        this->g_ = static_cast<f32>((rgba >> 16) & mask<u8>) / mask<f32>;
        this->b_ = static_cast<f32>((rgba >> 8) & mask<u8>) / mask<f32>;
        this->a_ = static_cast<f32>(rgba & mask<u8>) / mask<f32>;
      } else {
        this->r_ = static_cast<f32>((rgba >> 16) & mask<u8>) / mask<f32>;
        this->g_ = static_cast<f32>((rgba >> 8) & mask<u8>) / mask<f32>;
        this->b_ = static_cast<f32>(rgba & mask<u8>) / mask<f32>;
        this->a_ = 1.0F;
      }
    }

    /// \brief Constructs a color from string_view.
    /// \details The string must be in the one of the following formats:
    /// <ul>
    /// <li><code>#32a85280</code></li>
    /// <li><code>32a85280</code></li>
    /// <li><code>#32a852</code></li>
    /// <li><code>32a852</code></li>
    /// </ul>
    /// \throws std::invalid_argument if the string is not a valid color.
    /// \param s String view.
    constexpr explicit color(std::string_view s) {
      if(s.starts_with("#"))
        s = s.substr(1);
      if (s.size() == 6) {
        auto const rgb = detail::str_to_rgb(s.data());
        this->r_ = static_cast<f32>(rgb[0]) / mask<f32>;
        this->g_ = static_cast<f32>(rgb[1]) / mask<f32>;
        this->b_ = static_cast<f32>(rgb[2]) / mask<f32>;
        this->a_ = 1.0F;
      } else if (s.size() == 8) {
        auto const rgba = detail::str_to_rgba(s.data());
        this->r_ = static_cast<f32>(rgba[0]) / mask<f32>;
        this->g_ = static_cast<f32>(rgba[1]) / mask<f32>;
        this->b_ = static_cast<f32>(rgba[2]) / mask<f32>;
        this->a_ = static_cast<f32>(rgba[3]) / mask<f32>;
      } else {
        throw std::invalid_argument("invalid color: "  + std::string(s));
      }
    }

    /// \brief Constructs an opaque color from HSL values.
    /// \param hsl HSL color value.
    /// \see from_hsl
    constexpr explicit color(color::hsl_t const& hsl)
      : a_(1.0F) {
      auto const t = hsl.to_rgb();
      this->r_ = static_cast<f32>(t[0]) / mask<f32>;
      this->g_ = static_cast<f32>(t[1]) / mask<f32>;
      this->b_ = static_cast<f32>(t[2]) / mask<f32>;
    }

    /// \brief Constructs a color from HSLA values.
    /// \param hsla HSLA color value.
    /// \see from_hsla
    explicit constexpr color(color::hsla_t const& hsla) {
      auto const t = hsla.to_rgba();
      this->r_ = static_cast<f32>(t[0]) / mask<f32>;
      this->g_ = static_cast<f32>(t[1]) / mask<f32>;
      this->b_ = static_cast<f32>(t[2]) / mask<f32>;
      this->a_ = static_cast<f32>(t[3]) / mask<f32>;
    }

    /// \brief Constructs a color from HSV values.
    /// \param hsv HSV color value.
    /// \see from_hsv
    explicit constexpr color(color::hsv_t const& hsv)
      : a_(1.0F) {
      auto const t = hsv.to_rgb();
      this->r_ = static_cast<f32>(t[0]) / mask<f32>;
      this->g_ = static_cast<f32>(t[1]) / mask<f32>;
      this->b_ = static_cast<f32>(t[2]) / mask<f32>;
    }

    /// \brief Constructs a color from HSVA values.
    /// \param hsva HSVA color value.
    /// \see from_hsva
    constexpr explicit color(color::hsva_t const& hsva) {
      auto const t = hsva.to_rgba();
      this->r_ = static_cast<f32>(t[0]) / mask<f32>;
      this->g_ = static_cast<f32>(t[1]) / mask<f32>;
      this->b_ = static_cast<f32>(t[2]) / mask<f32>;
      this->a_ = static_cast<f32>(t[3]) / mask<f32>;
    }

    /// \brief Constructs a color from CMYK values.
    /// \param cmyk CMYK color value.
    /// \see from_cmyk
    constexpr explicit color(color::cmyk_t const& cmyk)
      : a_{1.0F} {
      auto const t = cmyk.to_rgb();
      this->r_ = static_cast<f32>(t[0]) / mask<f32>;
      this->g_ = static_cast<f32>(t[1]) / mask<f32>;
      this->b_ = static_cast<f32>(t[2]) / mask<f32>;
    }

    /// \brief Constructs a color from CMYKA values.
    /// \param cmyka CMYKA color value.
    /// \see from_cmyka
    constexpr explicit color(color::cmyka_t const& cmyka) {
      auto const t = cmyka.to_rgba();
      this->r_ = static_cast<f32>(t[0]) / mask<f32>;
      this->g_ = static_cast<f32>(t[1]) / mask<f32>;
      this->b_ = static_cast<f32>(t[2]) / mask<f32>;
      this->a_ = static_cast<f32>(t[3]) / mask<f32>;
    }


    /// \brief Returns the string representation of the color.
    /// \details Example: <code>#RRGGBBAA</code>.
    /// \returns The string representation of the color.
    [[nodiscard]] virtual auto to_string() const -> std::string override {
      return fmt::format("#{:02X}{:02X}{:02X}{:02X}",
        static_cast<u8>(r_ * mask<f32>),
        static_cast<u8>(g_ * mask<f32>),
        static_cast<u8>(b_ * mask<f32>),
        static_cast<u8>(a_ * mask<f32>)
      );
    }

    /// \brief Returns the red color component in range <code>0..255</code>.
    /// \returns The red color component in range <code>0..255</code>.
    /// \see redf()
    /// \see green()
    /// \see blue()
    /// \see alpha()
    /// \see rgb()
    [[nodiscard]] constexpr auto red() const -> u8 { return static_cast<u8>(this->r_ * mask<u8>); }

    /// \brief Returns the red color component in range <code>0..1</code>.
    /// \returns The red color component in range <code>0..1</code>.
    /// \see red()
    /// \see greenf()
    /// \see bluef()
    /// \see alphaf()
    /// \see rgb()
    [[nodiscard]] constexpr auto redf() const -> f32 { return this->r_; }

    /// \brief Returns the green color component in range <code>0..255</code>.
    /// \returns The green color component in range <code>0..255</code>.
    /// \see greenf()
    /// \see red()
    /// \see blue()
    /// \see alpha()
    /// \see rgb()
    [[nodiscard]] constexpr auto green() const -> u8 { return static_cast<u8>(this->g_ * mask<u8>); }

    /// \brief Returns the green color component in range <code>0..1</code>.
    /// \returns The green color component in range <code>0..1</code>.
    /// \see green()
    /// \see redf()
    /// \see bluef()
    /// \see alphaf()
    /// \see rgb()
    [[nodiscard]] constexpr auto greenf() const -> f32 { return this->g_; }

    /// \brief Returns the blue color component in range <code>0..255</code>.
    /// \returns The blue color component in range <code>0..255</code>.
    /// \see bluef()
    /// \see red()
    /// \see green()
    /// \see alpha()
    /// \see rgb()
    [[nodiscard]] constexpr auto blue() const -> u8 { return static_cast<u8>(this->b_ * mask<u8>); }

    /// \brief Returns the blue color component in range <code>0..1</code>.
    /// \returns The blue color component in range <code>0..1</code>.
    /// \see blue()
    /// \see redf()
    /// \see greenf()
    /// \see alphaf()
    /// \see rgb()
    [[nodiscard]] constexpr auto bluef() const -> f32 { return this->b_; }

    /// \brief Returns the alpha color component in range <code>0..255</code>.
    /// \returns The alpha color component in range <code>0..255</code>.
    /// \see alphaf()
    [[nodiscard]] constexpr auto alpha() const -> u8 { return static_cast<u8>(this->a_ * mask<u8>); }

    /// \brief Returns the alpha color component in range <code>0..1</code>.
    /// \returns The alpha color component in range <code>0..1</code>.
    /// \see alpha()
    [[nodiscard]] constexpr auto alphaf() const -> f32 { return a_; }

    /// \brief Returns the hue color component (HSL/HSV) in range <code>0..360</code>.
    /// \returns The hue color component (HSL/HSV) in range <code>0..255</code>.
    /// \see huef()
    [[nodiscard]] constexpr auto hue() const -> u8 { return static_cast<u8>(static_cast<f32>(this->hsl().h) * 360.0F); }

    /// \brief Returns the hue color component (HSL/HSV) in range <code>0..1</code>.
    /// \returns The hue color component (HSL/HSV) in range <code>0..1</code>.
    /// \see hue()
    [[nodiscard]] constexpr auto huef() const -> f32 { return this->hsl().h; }

    /// \brief Returns the lightness color component (HSL) in range <code>0..1</code>.
    /// \returns The lightness color component (HSL) in range <code>0..1</code>.
    /// \see lightness()
    /// \see hsl()
    [[nodiscard]] constexpr auto lightness() const -> f32 { return this->hsl().l; }

    /// \brief Returns the saturation color component (HSL) in range <code>0..1</code>.
    /// \returns The saturation color component (HSL) in range <code>0..1</code>.
    /// \note This saturation is not the same as the HSV saturation. Use explicit \ref hsv() function to access it.
    /// \see saturation()
    /// \see hsl()
    [[nodiscard]] constexpr auto saturation() const -> f32 { return this->hsl().s; }

    /// \brief Returns RGB color components as 32-bit unsigned integer.
    /// \details If you need the individual color components, use \ref red(), \ref green(), \ref blue(), \ref alpha() instead.
    /// \returns RGB color components as 32-bit unsigned integer.
    /// \see rgba()
    /// \see red()
    /// \see green()
    /// \see blue()
    /// \see alpha()
    [[nodiscard]] constexpr auto rgb() const -> u32 {
      return static_cast<u32>(r_ * mask<u8>) << 24
        | static_cast<u32>(g_ * mask<u8>) << 16
        | static_cast<u32>(b_ * mask<u8>) << 8
        | mask<u8>;
    }

    /// \brief Returns RGBA color components as 32-bit unsigned integer.
    /// \details If you need the individual color components, use \ref red(), \ref green(), \ref blue(), \ref alpha() instead.
    /// \returns RGBA color components as 32-bit unsigned integer.
    /// \see rgb()
    /// \see red()
    /// \see green()
    /// \see blue()
    /// \see alpha()
    [[nodiscard]] constexpr auto rgba() const -> u32 {
      return static_cast<u32>(r_ * mask<u8>) << 24
        | static_cast<u32>(g_ * mask<u8>) << 16
        | static_cast<u32>(b_ * mask<u8>) << 8
        | static_cast<u32>(a_ * mask<u8>);
    }

    /// \brief Returns HSL color components as <tt>color::hsl_t</tt> type.
    /// \details If you need the individual color components, use \ref hue(), \ref saturation(), \ref lightness() instead.
    /// \returns HSL color components as <tt>color::hsl_t</tt> type.
    /// \see hsla()
    /// \see hsv()
    /// \see cmyk()
    /// \see rgb()
    [[nodiscard]] constexpr auto hsl() const -> color::hsl_t {
      return color::hsl_t::from_rgb(this->red(), this->green(), this->blue());
    }

    /// \brief Returns HSLA color components as <tt>color::hsla_t</tt> type.
    /// \details If you need the individual color components, use \ref hue(), \ref saturation(), \ref lightness(), \ref alpha() instead.
    /// \returns HSLA color components as <tt>color::hsla_t</tt> type.
    /// \see hsl()
    /// \see hsva()
    /// \see cmyka()
    /// \see rgba()
    [[nodiscard]] constexpr auto hsla() const -> color::hsla_t {
      auto const hsl = this->hsl();
      return {
        hsl.h,
        hsl.s,
        hsl.l,
        this->alphaf()
      };
    }

    /// \brief Returns HSV color components as <tt>color::hsv_t</tt> type.
    /// \details If you need the individual color components, use \ref hue(), \ref saturation(), \ref value() instead.
    /// \returns HSV color components as <tt>color::hsv_t</tt> type.
    /// \see hsva()
    /// \see cmyk()
    /// \see rgb()
    [[nodiscard]] constexpr auto hsv() const -> color::hsv_t {
      return color::hsv_t::from_rgb(this->red(), this->green(), this->blue());
    }

    /// \brief Returns HSVA color components as <tt>color::hsva_t</tt> type.
    /// \details If you need the individual color components, use \ref hue(), \ref saturation(), \ref value(), \ref alpha() instead.
    /// \returns HSVA color components as <tt>color::hsva_t</tt> type.
    /// \see hsv()
    /// \see cmyka()
    /// \see rgba()
    [[nodiscard]] constexpr auto hsva() const -> color::hsva_t {
      auto const hsv = this->hsv();
      return {
        hsv.h,
        hsv.s,
        hsv.v,
        this->alphaf()
      };
    }

    /// \brief Returns CMYK color components as <tt>color::cmyk_t</tt> type.
    /// \details If you need the individual color components, use \ref cyan(), \ref magenta(), \ref yellow(), \ref black() instead.
    /// \returns CMYK color components as <tt>color::cmyk_t</tt> type.
    /// \see cmyka()
    /// \see rgb()
    [[nodiscard]] constexpr auto cmyk() const -> color::cmyk_t {
      return color::cmyk_t::from_rgb(this->red(), this->green(), this->blue());
    }

    /// \brief Returns CMYKA color components as <tt>color::cmyka_t</tt> type.
    /// \details If you need the individual color components, use \ref cyan(), \ref magenta(), \ref yellow(), \ref black(), \ref alpha() instead.
    /// \returns CMYKA color components as <tt>color::cmyka_t</tt> type.
    /// \see cmyk()
    /// \see rgba()
    [[nodiscard]] constexpr auto cmyka() const -> color::cmyka_t {
      return color::cmyka_t::from_rgba(this->red(), this->green(), this->blue(), this->alpha());
    }

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Returns the color as <tt>QColor</tt> object.
    /// \returns The color as <tt>QColor</tt> object.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    /// \see from_qcolor()
    /// \sa https://doc.qt.io/qt-6/qcolor.html
    [[nodiscard]] constexpr auto to_qcolor() const -> QColor {
      return {
        this->red(),
        this->green(),
        this->blue(),
        this->alpha()
      };
    }
  #endif

    [[nodiscard]] constexpr auto operator==(color const& other) const -> bool {
      return math::approx_eq(this->r_, other.r_, 3.0F)
        and math::approx_eq(this->g_, other.g_, 3.0F)
        and math::approx_eq(this->b_, other.b_, 3.0F)
        and math::approx_eq(this->a_, other.a_, 3.0F);
    }

    [[nodiscard]] constexpr auto operator!=(color const& other) const -> bool { return not (*this == other); }

    [[nodiscard]] constexpr auto operator+(color const& other) const -> color {
      return {
        static_cast<u8>(this->red() + other.red()),
        static_cast<u8>(this->green() + other.green()),
        static_cast<u8>(this->blue() + other.blue()),
        static_cast<u8>(this->alpha() + other.alpha())
      };
    }

    [[nodiscard]] constexpr auto operator-(color const& other) const -> color {
      return {
        static_cast<u8>(this->red() - other.red()),
        static_cast<u8>(this->green() - other.green()),
        static_cast<u8>(this->blue() - other.blue()),
        static_cast<u8>(this->alpha() - other.alpha())
      };
    }

    [[nodiscard]] constexpr auto operator*(color const& other) const -> color {
      return {
        static_cast<u8>(this->red() * other.red()),
        static_cast<u8>(this->green() * other.green()),
        static_cast<u8>(this->blue() * other.blue()),
        static_cast<u8>(this->alpha() * other.alpha())
      };
    }

    [[nodiscard]] constexpr auto operator/(color const& other) const -> color {
      return {
        static_cast<u8>(this->red() / other.red()),
        static_cast<u8>(this->green() / other.green()),
        static_cast<u8>(this->blue() / other.blue()),
        static_cast<u8>(this->alpha() / other.alpha())
      };
    }

    /// wtf ???
    [[nodiscard]] constexpr auto operator-() const -> color {
      return color(-this->red(), -this->green(), -this->blue(), -this->alpha());
    }

    [[nodiscard]] constexpr auto operator*=(color const& other) -> color& {
      this->r_ *= other.r_;
      this->g_ *= other.g_;
      this->b_ *= other.b_;
      this->a_ *= other.a_;
      return *this;
    }

    [[nodiscard]] constexpr auto operator/=(color const& other) -> color& {
      this->r_ /= other.r_;
      this->g_ /= other.g_;
      this->b_ /= other.b_;
      this->a_ /= other.a_;
      return *this;
    }

    [[nodiscard]] constexpr auto operator+=(color const& other) -> color& {
      this->r_ += other.r_;
      this->g_ += other.g_;
      this->b_ += other.b_;
      this->a_ += other.a_;
      return *this;
    }

    [[nodiscard]] constexpr auto operator-=(color const& other) -> color& {
      this->r_ -= other.r_;
      this->g_ -= other.g_;
      this->b_ -= other.b_;
      this->a_ -= other.a_;
      return *this;
    }

    [[nodiscard]] constexpr auto operator*=(const f32 other) -> color& {
      this->r_ *= other;
      this->g_ *= other;
      this->b_ *= other;
      this->a_ *= other;
      return *this;
    }

    [[nodiscard]] constexpr auto operator/=(const f32 other) -> color& {
      this->r_ /= other;
      this->g_ /= other;
      this->b_ /= other;
      this->a_ /= other;
      return *this;
    }

    /// \brief Constructs an color from RGBA values in the range [0.0, 1.0].
    /// \param r Red component.
    /// \param g Green component.
    /// \param b Blue component.
    /// \param a Alpha component.
    [[nodiscard]] static constexpr auto from_floats(const f32 r, const f32 g, const f32 b, const f32 a) -> color {
      return {
        static_cast<u8>(r * mask<f32>),
        static_cast<u8>(g * mask<f32>),
        static_cast<u8>(b * mask<f32>),
        static_cast<u8>(a * mask<f32>)
      };
    }

    /// \brief Constructs an opaque color from HSL values.
    /// \param hsl HSL color value.
    [[nodiscard]] static constexpr auto from_hsl(color::hsl_t const& hsl) -> color {
      return color(hsl);
    }

    /// \brief Constructs a color from HSLA values.
    /// \param hsla HSLA color value.
    [[nodiscard]] static constexpr auto from_hsla(color::hsla_t const& hsla) -> color {
      return color(hsla);
    }

    /// \brief Construct a color from HSV values.
    /// \param hsv HSV color value.
    [[nodiscard]] static constexpr auto from_hsv(color::hsv_t const& hsv) -> color {
      return color(hsv);
    }

    /// \brief Construct a color from HSVA values.
    /// \param hsva HSVA color value.
    [[nodiscard]] static constexpr auto from_hsva(color::hsva_t const& hsva) -> color {
      return color(hsva);
    }

    /// \brief Construct a color from CMYK values.
    /// \param cmyk CMYK color value.
    [[nodiscard]] static constexpr auto from_cmyk(color::cmyk_t const& cmyk) -> color {
      return color(cmyk);
    }

    /// \brief Construct a color from CMYKA values.
    /// \param cmyka CMYKA color value.
    [[nodiscard]] static constexpr auto from_cmyka(color::cmyka_t const& cmyka) -> color {
      return color(cmyka);
    }

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Constructs a color from a <b>QColor</b> class instance.
    /// \param col <b>QColor</b> class instance.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    /// \sa https://doc.qt.io/qt-5/qcolor.html
    [[nodiscard]] static auto from_qcolor(QColor const& col) -> color {
      return from_floats(
        static_cast<f32>(col.redF()),
        static_cast<f32>(col.greenF()),
        static_cast<f32>(col.blueF()),
        static_cast<f32>(col.alphaF())
      );
    }
  #endif // FL_QT_GUI || FL_DOC

   private:
    f32 r_;
    f32 g_;
    f32 b_;
    f32 a_ {1.0F};
  };

  static_assert(color(50, 168, 82).rgb() == color(0x32A852FF).rgb());
  static_assert(color("#32a852").rgb() == color(0x32A852FF).rgb());
  static_assert(color("32a852").rgb() == color(0x32A852FF).rgb());
  static_assert(color(50, 168, 82, 128).rgba() == color(0x32A85280).rgba());
  static_assert(color("#32a85280").rgba() == color(0x32A85280).rgba());
  static_assert(color("32a85280").rgba() == color(0x32A85280).rgba());
  static_assert(color::from_floats(0.1966F, 0.659F, 0.322F, 0.505F).rgba() == color("32a85280").rgba());
} // namespace floppy::gfx