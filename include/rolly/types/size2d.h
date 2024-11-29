#pragma once

#include "../concepts/num.h"
#include "./stdint.h"
#include <string>
#include <array>
#include <tuple>
#include <algorithm>

#if defined(ROLLY_QT_GUI)
#  include <qsize.h>
#endif

namespace rolly {
  inline namespace types {
    template <___concept___(concepts::num) T>
    class point2d;

    template <___concept___(concepts::num) T>
    class vector2d;

    /**
     * @brief A two-dimensional size2d tagged with a unit.
     * @tparam T Number type. Must satisfy concept <tt>rolly::concepts::num</tt>. Default is \c f32.
     * @see rolly::types::length
     * @see rolly::types::point2d
     * @see rolly::types::vector2d
     */
#ifdef DOXYGEN_GENERATING_OUTPUT
    template <concepts::num T = f32>
#else
    template <___concept___(concepts::num) T = f32>
#endif
    struct size2d {
      /**
       * @brief Underlying number type.
       */
      using number_type = T;

      /**
       * @brief Default copy constructor.
       */
      constexpr size2d(size2d const&) = default;

      /**
       * @brief Default copy assignment operator.
       */
      constexpr size2d& operator=(size2d const&) = default;

      /**
       * @brief Default move constructor.
       */
      constexpr size2d(size2d&&) = default;

      /**
       * @brief Default move assignment operator.
       */
      constexpr size2d& operator=(size2d&&) = default;

      /**
       * @brief Constructs new size2d with zero coordinates.
       */
      constexpr size2d()
        : x_(static_cast<number_type>(0))
        , y_(static_cast<number_type>(0)) {}

      /**
       * @brief Constructs new size2d from given scalar values.
       * @param x The x-coordinate.
       * @param y The y-coordinate.
       */
      constexpr size2d(number_type x, number_type y)
        : x_(x)
        , y_(y) {}

      /**
       * @brief Constructs new size2d, setting all components to the same value.
       * @param value The value to set all components to.
       */
      constexpr explicit size2d(number_type value)
        : x_(value)
        , y_(value) {}

      /**
       * @brief Constructs new size2d from a point2d.
       * @param p The point2d to copy.
       */
      constexpr explicit size2d(point2d<number_type> const& p)
        : x_(p.x())
        , y_(p.y()) {}

      /**
       * @brief Constructs new size2d from a vector2d.
       * @param v The vector2d to copy.
       */
      constexpr explicit size2d(vector2d<number_type> const& v)
        : x_(v.x())
        , y_(v.y()) {}

      /**
       * @brief Returns inverted size2d.
       * @return Inverted size2d.
       */
      [[nodiscard]] constexpr size2d inverted() const { return T(this->y_, this->x_); }

      /**
       * @brief Returns the x-coordinate of the size2d as scalar value.
       * @return The x-coordinate of the size2d.
       */
      [[nodiscard]] constexpr number_type x() const { return this->x_; }

      /**
       * @brief Returns the y-coordinate of the size2d as scalar value.
       * @return The y-coordinate of the size2d.
       */
      [[nodiscard]] constexpr number_type y() const { return this->y_; }

      /**
       * @brief Returns mutable reference to the x-coordinate of the size2d as scalar value.
       * @return Mutable reference to the x-coordinate of the size2d.
       */
      [[nodiscard]] constexpr number_type& x_mut() { return this->x_; }

      /**
       * @brief Returns mutable reference to the y-coordinate of the size2d as scalar value.
       * @return Mutable reference to the y-coordinate of the size2d.
       */
      [[nodiscard]] constexpr number_type& y_mut() { return this->y_; }

      /**
       * @brief Alias to <tt>x</tt> member function.
       */
      [[nodiscard]] constexpr number_type width() const { return this->x_; }

      /**
       * @brief Alias to <tt>y</tt> member function.
       */
      [[nodiscard]] constexpr number_type height() const { return this->y_; }

      /**
       * @brief Alias to <tt>x_mut</tt> member function.
       */
      [[nodiscard]] constexpr number_type& width_mut() { return this->x_; }

      /**
       * @brief Alias to <tt>y_mut</tt> member function.
       */
      [[nodiscard]] constexpr number_type& height_mut() { return this->y_; }

      /**
       * @brief Returns string representation of the size2d.
       * @details Size is represented as it's numeric values. If the underlying number type is
       * floating point2d, it is rounded to two decimal places.
       * @note Due to limitations of the language, units are not displayed.
       * @return String representation of the object.
       */
      [[nodiscard]] std::string to_string() const {
        if constexpr(std::is_floating_point_v<T>)
          return fmt::format("[{:.2f}, {:.2f}]", this->x_, this->y_);
        else
          return fmt::format("[{}, {}]", this->x_, this->y_);
      }

      /**
       * @brief Casts into an array of two values.
       */
      [[nodiscard]] constexpr std::array<number_type, 2> to_array() const {
        return {this->x_, this->y_};
      }

      /**
       * @brief Casts into a tuple of two values.
       */
      [[nodiscard]] constexpr std::tuple<number_type, number_type> to_tuple() const {
        return {this->x_, this->y_};
      }

      /**
       * @brief Casts into a pair of two values.
       */
      [[nodiscard]] constexpr std::pair<number_type, number_type> to_pair() const {
        return {this->x_, this->y_};
      }

      /**
       * @brief Rounds each component to the nearest integer value.
       * @details This behavior is preserved for negative values (unlike the basic cast).
       */
      [[nodiscard]] constexpr size2d round() const {
        return {std::round(this->x_), std::round(this->y_)};
      }

      /**
       * @brief Rounds each component to the smallest integer equal or greater than the original
       * value.
       * @details This behavior is preserved for negative values (unlike the basic cast).
       */
      [[nodiscard]] constexpr size2d ceil() const {
        return {std::ceil(this->x_), std::ceil(this->y_)};
      }

      /*
       * @brief Rounds each component to the largest integer equal or less than the original value.
       * @details This behavior is preserved for negative values (unlike the basic cast).
       */
      [[nodiscard]] constexpr size2d floor() const {
        return {std::floor(this->x_), std::floor(this->y_)};
      }

      /**
       * @brief Linearly interpolate between this size2d and another size2d.
       * @details Example:
       * @code {.cpp}
       * using rolly::types::size2d;
       * auto const from = size2d(0.0F, 10.0F);
       * auto const to = size2d(8.0F, -4.0F);
       * fmt::println("{}", from.lerp(to, -1.0F));
       * fmt::println("{}", from.lerp(to, 0.0F));
       * fmt::println("{}", from.lerp(to, 0.5F));
       * fmt::println("{}", from.lerp(to, 1.0F));
       * fmt::println("{}", from.lerp(to, 2.0F));
       * @endcode
       *
       * Output:
       * @code {.shell-session}
       * [-8.0, 24.0]
       * [0.0, 10.0]
       * [4.0, 3.0]
       * [8.0, -4.0]
       * [16.0, -18.0]
       * @endcode
       * @param other The other size2d.
       * @param t Interpolation factor.
       * @return The interpolated size2d.
       */
      [[nodiscard]] constexpr size2d lerp(size2d const& other, f32 t) const {
        return {this->x_ + ((other.x() - this->x_) * t), this->y_ + ((other.y() - this->y_) * t)};
      }

      /**
       * @brief Returns the largest x and y values of this and another size2d.
       * @param other The other size2d.
       * @return size2d with the largest x and y values.
       */
      [[nodiscard]] constexpr size2d max(size2d const& other) const {
        return {std::max(this->x_, other.x()), std::max(this->y_, other.y())};
      }

      /**
       * @brief Returns the smallest x and y values of this and another size2d.
       * @param other The other size2d.
       * @return size2d with the smallest x and y values.
       */
      [[nodiscard]] constexpr size2d min(size2d const& other) const {
        return std::min(this->x_, other.x()), std::min(this->y_, other.y());
      }

      /**
       * @brief Returns the size2d each component of which clamped by corresponding components of
       * start and end.
       * @param start The start size2d.
       * @param end The end size2d.
       * @return The clamped size2d.
       */
      [[nodiscard]] constexpr size2d clamp(size2d const& start, size2d const& end) const {
        return {std::clamp(this->x_, start.x_, end.x_), std::clamp(this->y_, start.y_, end.y_)};
      }

      /**
       * @brief Returns the absolute value of each component.
       * @return The absolute value of each component.
       */
      [[nodiscard]] constexpr size2d abs() const {
        return {std::abs(this->x_), std::abs(this->y_)};
      }

      /**
       * @brief Returns dot product of this and another size2d.
       * @details Dot product is defined as <code>[x1, y1] . [x2, y2] = x1 * x2 + y1 * y2</code>.
       * @param other The other size2d.
       * @return The dot product of this and another size2d.
       */
      [[nodiscard]] constexpr number_type dot(size2d const& other) const {
        return (this->x_ * other.x()) + (this->y_ * other.y());
      }

      /**
       * @brief Returns the norm of the cross product of this and another size2d.
       * @details Cross product is defined as <code>[x1, y1] x [x2, y2] = x1 * y2 - y1 * x2</code>.
       * @param other The other size2d.
       * @return The norm of the cross product of this and another size2d.
       */
      [[nodiscard]] constexpr number_type cross(size2d const& other) const {
        return (this->x_ * other.y()) - (this->y_ * other.x());
      }

      /**
       * @brief Returns true if all members are finite.
       */
      [[nodiscard]] constexpr bool is_finite() const {
        return std::isfinite(this->x_) && std::isfinite(this->y_);
      }

      /**
       * @brief Returns true if all members are positive.
       */
      [[nodiscard]] constexpr bool is_positive() const {
        return this->x_ > 0.0F and this->y_ > 0.0F;
      }

      /**
       * @brief The same as <i>origin</i>.
       */
      [[nodiscard]] static constexpr size2d zero() { return {}; }

      /**
       * @brief Constructs new size2d, setting all components to the same value.
       * @param value The value to set all components to.
       */
      [[nodiscard]] static constexpr size2d splat(number_type value) { return {value}; }

      /**
       * @brief Constructs new size2d from <tt>std::tuple</tt>.
       * @param other The other <tt>std::tuple</tt>.
       */
#ifdef DOXYGEN_GENERATING_OUTPUT
      template <typename... Args>
#else
      template <typename... Args ___sfinae_requirement___(std::tuple_size_v<std::tuple<Args...>> == 2)>
      ___requires___((std::tuple_size_v<std::tuple<Args...>> == 2))
#endif
      [[nodiscard]] static constexpr size2d from_tuple(std::tuple<Args...> const& other) {
        return {std::get<0>(other), std::get<1>(other)};
      }

      /**
       * @brief Constructs new size2d from <tt>std::array</tt>.
       * @tparam N2 The size of the array. Must be equal to <tt>2</tt>.
       * @param other The other <tt>std::array</tt>.
       */
#ifdef DOXYGEN_GENERATING_OUTPUT
      template <std::size_t N2>
        requires(N2 == 2)
#else
      template <std::size_t N2 ___sfinae_requirement___(N2 == 2)> ___requires___((N2 == 2))
#endif
      [[nodiscard]] static constexpr size2d from_array(std::array<number_type, N2> const& other) {
        return {other[0], other[1]};
      }

      /**
       * @brief Constructs new size2d from <tt>std::pair</tt>.
       * @param other The other <tt>std::pair</tt>.
       */
      [[nodiscard]] static constexpr size2d from_pair(
        std::pair<number_type, number_type> const& other
      ) {
        return {other.first, other.second};
      }

      /**
       * @brief Returns <tt>true</tt> if underlying values is both not <tt>0</tt> and not
       * <tt>NaN</tt> or <tt>Infinity</tt> in case of floating point. \note Also returns
       * <tt>false</tt> if underlying values is <tt>NaN</tt> or <tt>Infinity</tt> in case of
       * floating point.
       */
      [[nodiscard]] constexpr explicit operator bool() const {
        if constexpr(std::is_floating_point_v<number_type>) {
          if(not this->is_finite())
            return false;
          if(std::isnan(this->x_) or std::isnan(this->y_))
            return false;
        }
        return not is_null(this->x_) and not is_null(this->y_);
      }

      /**
       * @brief Returns <tt>false</tt> if underlying values is both not <tt>0</tt>.
       */
      [[nodiscard]] constexpr bool operator!() const { return not this->operator bool(); }

      /**
       * @brief Converts this size2d into <tt>point2d</tt>.
       * @return The resulting point2d.
       */
      [[nodiscard]] constexpr point2d<number_type> to_point2d() const {
        return {this->x_, this->y_};
      }

      /**
       * @brief Converts this size2d into <tt>vector2d</tt>.
       * @return The resulting vector2d.
       */
      [[nodiscard]] constexpr vector2d<number_type> to_vector2d() const {
        return {this->x_, this->y_};
      }

      /**
       * @brief Cast from one numeric representation to another, preserving the units.
       * @tparam T2 New number type.
       * @return The size2d with the new number type and the same value.
       */
      template <___concept___(concepts::num) T2>
      [[nodiscard]] constexpr size2d<T2> cast() const {
        return {this->x_, this->y_};
      }

      /**
       * @brief Casts into <tt>f32</tt> size2d.
       */
      [[nodiscard]] constexpr size2d<> to_f32() const { return {this->x_, this->y_}; }

      /**
       * @brief Casts into <tt>f64</tt> size2d.
       */
      [[nodiscard]] constexpr size2d<f64> to_f64() const { return {this->x_, this->y_}; }

      /**
       * @brief Casts into <tt>u32</tt> size2d.
       */
      [[nodiscard]] constexpr size2d<u32> to_u32() const { return {this->x_, this->y_}; }

      /**
       * @brief Casts into <tt>u64</tt> size2d.
       */
      [[nodiscard]] constexpr size2d<u64> to_u64() const { return {this->x_, this->y_}; }

      /**
       * @brief Casts into <tt>usize</tt> size2d.
       */
      [[nodiscard]] constexpr size2d<usize> to_usize() const { return {this->x_, this->y_}; }

      /**
       * @brief Casts into <tt>i32</tt> size2d.
       */
      [[nodiscard]] constexpr size2d<i32> to_i32() const { return {this->x_, this->y_}; }

      /**
       * @brief Casts into <tt>i64</tt> size2d.
       */
      [[nodiscard]] constexpr size2d<i64> to_i64() const { return {this->x_, this->y_}; }

      /**
       * @brief Casts into <tt>isize</tt> size2d.
       */
      [[nodiscard]] constexpr size2d<isize> to_isize() const { return {this->x_, this->y_}; }

      /**
       * @brief Returns result of multiplication of both scalars.
       * @return Area of the size2d.
       */
      [[nodiscard]] constexpr number_type area() const { return this->x_ * this->y_; }

      /**
       * @brief Returns the unchanged size2d.
       */
      [[nodiscard]] constexpr size2d operator+() const { return *this; }

      /**
       * @brief Returns the negated size2d.
       */
      [[nodiscard]] constexpr size2d operator-() const { return size2d(-this->x_, -this->y_); }

      /**
       * @brief Equality comparison.
       * @param other The other size2d.
       * @return <tt>true</tt> if the size2ds are equal, <tt>false</tt> otherwise.
       */
      [[nodiscard]] constexpr bool operator==(size2d const& other) const {
        return approx_eq(this->x_, other.x()) and approx_eq(this->y_, other.y());
      }

      /**
       * @brief Inequality comparison.
       * @param other The other size2d.
       * @return <tt>true</tt> if the size2ds are not equal, <tt>false</tt> otherwise.
       */
      [[nodiscard]] constexpr bool operator!=(size2d const& other) const {
        return not eq(this->x_, other.x()) or not eq(this->y_, other.y());
      }

      /**
       * @brief Addition operator.
       * @param other The other size2d.
       * @return The sum of both size2ds.
       */
      [[nodiscard]] constexpr size2d operator+(size2d const& other) const {
        return {this->x_ + other.x(), this->y_ + other.y()};
      }

      /**
       * @brief Subtraction operator.
       * @param other The other size2d.
       * @return The difference of both size2ds.
       */
      [[nodiscard]] constexpr size2d operator-(size2d const& other) const {
        return {this->x_ - other.x(), this->y_ - other.y()};
      }

      /**
       * @brief Multiplies size2d with a scalar.
       * @param other Scalar value.
       * @return The product of the size2d and the scalar.
       */
      [[nodiscard]] constexpr size2d operator*(number_type const& other) const {
        return {this->x_ * other, this->y_ * other};
      }

      /**
       * @brief Divides size2d by a scalar.
       * @param other Scalar value.
       * @return The quotient of the size2d and the scalar.
       */
      [[nodiscard]] constexpr size2d operator/(number_type const& other) const {
        return {this->x_ / other, this->y_ / other};
      }

      /**
       * @brief Addition assignment operator.
       * @param other The other size2d.
       * @return The result of the addition.
       */
      constexpr size2d& operator+=(size2d const& other) {
        this->width_mut() += other.x();
        this->height_mut() += other.y();
        return *this;
      }

      /**
       * @brief Subtraction assignment operator.
       * @param other The other size2d.
       * @return The result of the subtraction.
       */
      constexpr size2d& operator-=(size2d const& other) {
        this->width_mut() -= other.x();
        this->height_mut() -= other.y();
        return *this;
      }

      /**
       * @brief Multiplication assignment operator.
       * @param other Scalar value.
       * @return The result of the multiplication.
       */
      constexpr size2d& operator*=(number_type const& other) {
        this->width_mut() *= other;
        this->height_mut() *= other;
        return *this;
      }

      /**
       * @brief Division assignment operator.
       * @param other Scalar value.
       * @return The result of the division.
       */
      constexpr size2d& operator/=(number_type const& other) {
        this->width_mut() /= other;
        this->height_mut() /= other;
        return *this;
      }

#if defined(ROLLY_QT_GUI) || defined(FL_DOC)
      /**
       * @brief Constructs new size2d from <tt>QSize</tt>.
       * @param other The other <tt>QSize</tt>.
       * @remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this
       * header is compiled for. \sa https://doc.qt.io/qt-5/qsize.html
       */
      constexpr explicit size2d(QSize const& other)
        : size2d(other.width(), other.height()) {}

      /**
       * @brief Constructs new size2d from <tt>QSizeF</tt>.
       * @param other The other <tt>QSizeF</tt>.
       * @remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this
       * header is compiled for. \sa https://doc.qt.io/qt-5/qsizef.html
       */
      constexpr explicit size2d(QSizeF const& other)
        : size2d(other.width(), other.height()) {}

      /**
       * @brief Constructs new size2d from <tt>QSize</tt>.
       * @param other The other <tt>QSize</tt>.
       * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
       * header is compiled for. \sa https://doc.qt.io/qt-5/qsize.html
       */
      [[nodiscard]] static constexpr size2d from_qsize(QSize const& other) {
        return size2d(other.width(), other.height());
      }

      /**
       * @brief Constructs new size2d from <tt>QSizeF</tt>.
       * @param other The other <tt>QSizeF</tt>.
       * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
       * header is compiled for. \sa https://doc.qt.io/qt-5/qsizef.html
       */
      [[nodiscard]] static constexpr size2d from_qsizef(QSizeF const& other) {
        return size2d(other.width(), other.height());
      }

      /**
       * @brief Casts this size2d into <tt>QSize</tt>.
       * @return The resulting <tt>QSize</tt>.
       * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
       * header is compiled for. \sa https://doc.qt.io/qt-5/qsize.html
       */
      [[nodiscard]] constexpr QSize to_qsize() const {
        return QSize(static_cast<i32>(this->x_), static_cast<i32>(this->y_));
      }

      /**
       * @brief Casts this size2d into <tt>QSizeF</tt>.
       * @return The resulting <tt>QSizeF</tt>.
       * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
       * header is compiled for. \sa https://doc.qt.io/qt-5/qsizef.html
       */
      [[nodiscard]] constexpr QSizeF to_qsizef() const { return QSizeF(this->x_, this->y_); }
#endif

     private:
      number_type x_;  //< The underlying x-coordinate.
      number_type y_;  //< The underlying y-coordinate.
    };
  }  // namespace types
}  // namespace rolly

/**
 * @brief Specialization of the <code>fmt::formatter</code> for the @ref rolly::size2d class.
 * @tparam T Number type.
 * @relates rolly::size2d
 */
template <typename T>
struct fmt::formatter<rolly::size2d<T>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  auto format(rolly::size2d<T> const& val, format_context& ctx) const {
    fmt::format_to(ctx.out(), "{}", val.to_string());
    return ctx.out();
  }
};

namespace std {
  /**
   * @brief Hashes a <tt>size2d</tt>.
   * @tparam T Underlying type of the <tt>size2d</tt>.
   * @param b <tt>size2d</tt> to hash.
   * @return Hash value.
   * @relates rolly::types::size2d
   * @sa http://en.cppreference.com/w/cpp/utility/hash
   */
  template <typename T>
  struct hash<rolly::types::size2d<T>> {
    size_t operator()(rolly::types::size2d<T> const& b) const {
      return std::hash<T> {}(b.x()) xor std::hash<T> {}(b.y());
    }
  };
}  // namespace std
