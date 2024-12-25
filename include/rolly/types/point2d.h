#pragma once

#include <string>
#include <array>
#include <tuple>
#include <algorithm>
#include <fmt/format.h>
#include "../math.h"
#include "../concepts/num.h"
#include "../concepts/any_of.h"
#include "./size2d.h"
#include "./vector2d.h"
#include "./stdint.h"

#if defined(ROLLY_QT_GUI)
#  include <qpoint.h>
#endif

namespace rolly {
#ifdef DOXYGEN
  template <concepts::num T = f32>
#else
  template <___concept___(concepts::num) T = f32>
#endif
  /**
   * @brief A two-dimensional point tagged with a unit.
   * @tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c
   * f32.
   * @see rolly::size2d
   * @see rolly::vector2d
   */
  struct point2d {
    /**
     * @brief Underlying number type.
     */
    using number_type = T;

    /**
     * @brief Associated size2d type.
     */
    using size2d_type = size2d<number_type>;

    /**
     * @brief Associated vector type.
     */
    using vector2d_type = vector2d<number_type>;

    /**
     * @brief Constructs new point with zero coordinates.
     */
    constexpr point2d()
      : x_(static_cast<number_type>(0))
      , y_(static_cast<number_type>(0)) {}

    /**
     * @brief Constructs new point from given scalar values.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     */
    constexpr point2d(number_type x, number_type y)
      : x_(x)
      , y_(y) {}

    /**
     * @brief Constructs new point, setting all components to the same value.
     * @param value The value to set all components to.
     */
    constexpr explicit point2d(number_type value)
      : x_(value)
      , y_(value) {}

    /**
     * @brief Constructs new point from size2d.
     * @param other The other size2d.
     */
    constexpr explicit point2d(size2d_type const& other)
      : point2d(other.x(), other.y()) {}

    /**
     * @brief Constructs new point from a vector2d.
     * @param v The vector2d to copy.
     */
    constexpr explicit point2d(vector2d<number_type> const& v)
      : x_(v.x())
      , y_(v.y()) {}

    /**
     * @brief Applies the function <b>fn</b> to each component of the point.
     * @tparam F The type of function to apply.
     * @param fn The function to apply.
     */
#ifdef DOXYGEN
    template <std::invocable<number_type> F>
#else
    template <___concept___(std::invocable<number_type>) F>
#endif
    constexpr auto map(F&& fn) const {
      return point2d<decltype(fn(this->x_))>(fn(this->x_), fn(this->y_));
    }

    /**
     * @brief Applies the function <b>fn</b> to each component of this point and the other point.
     * @tparam F The type of function to apply.
     * @param other The other point to apply.
     * @param fn The function to apply.
     */
    template <
#ifdef ___rolly_cxx20___
      std::invocable<number_type, number_type>
#else
      typename
#endif
        F>
    constexpr auto zip(point2d const& other, F&& fn) const {
      using result_type = decltype(fn(std::declval<number_type>(), std::declval<number_type>()));
      return point2d<result_type>(fn(this->x_, other.x()), fn(this->y_, other.y()));
    }

    /**
     * @brief Converts this point2d into <tt>size2d</tt>.
     * @return The resulting size2d.
     */
    [[nodiscard]] constexpr size2d_type to_size2d() const { return {this->x(), this->y()}; }

    /**
     * @brief Converts this point2d into <tt>vector2d</tt>.
     * @return The resulting vector2d.
     */
    [[nodiscard]] constexpr vector2d<number_type> to_vector2d() const {
      return vector2d<number_type>(this->x(), this->y());
    }

    /**
     * @brief Casts into <tt>f32</tt> point2d.
     */
    [[nodiscard]] constexpr point2d<> to_f32() const { return point2d<>(this->x_, this->y_); }

    /**
     * @brief Casts into <tt>f64</tt> point2d.
     */
    [[nodiscard]] constexpr point2d<f64> to_f64() const { return point2d<f64>(this->x_, this->y_); }

    /**
     * @brief Casts into <tt>u32</tt> point2d.
     */
    [[nodiscard]] constexpr point2d<u32> to_u32() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>u64</tt> point2d.
     */
    [[nodiscard]] constexpr point2d<u64> to_u64() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>usize</tt> point2d.
     */
    [[nodiscard]] constexpr point2d<usize> to_usize() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>i32</tt> point2d.
     */
    [[nodiscard]] constexpr point2d<i32> to_i32() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>i64</tt> point2d.
     */
    [[nodiscard]] constexpr point2d<i64> to_i64() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>isize</tt> point2d.
     */
    [[nodiscard]] constexpr point2d<isize> to_isize() const { return {this->x_, this->y_}; }

    /**
     * @brief Calculates Euclidean division, the matching method for rem_euclid.
     * @tparam Q The type of the divisor. Can be <tt>point2d</tt> or <tt>size2d</tt>.
     * @param other The divisor.
     * @return The quotient.
     * @see rolly::math::rem_euclid, rolly::math::div_euclid
     * @see rem_euclid
     */
    template <
#ifdef ___rolly_cxx20___
      concepts::any_of<point2d, size2d_type>
#else
      typename
#endif
        Q>
    [[nodiscard]] constexpr point2d div_euclid(Q const& other) const {
      return point2d(
        rolly::div_euclid(this->x_, other.x()),
        rolly::div_euclid(this->y_, other.y())
      );
    }

    /**
     * @brief Calculates the least nonnegative remainder of <tt>self (mod other)</tt>.
     * @tparam Q The type of the divisor. Can be <tt>point2d</tt> or <tt>size2d</tt>.
     * @param other The divisor.
     * @return The remainder.
     * @see rolly::math::rem_euclid, rolly::math::div_euclid
     * @see div_euclid
     */
    template <
#ifdef ___rolly_cxx20___
      concepts::any_of<point2d, size2d_type>
#else
      typename
#endif
        Q>
    [[nodiscard]] constexpr point2d rem_euclid(Q const& other) const {
      return point2d(
        rolly::rem_euclid(this->x_, other.x()),
        rolly::rem_euclid(this->y_, other.y())
      );
    }

    /**
     * @brief Constructs new point with zero coordinates.
     */
    [[nodiscard]] static constexpr point2d origin() { return {}; }

    /**
     * @brief Returns the unchanged point.
     */
    [[nodiscard]] constexpr point2d operator+() const { return *this; }

    /**
     * @brief Returns the negated point.
     */
    [[nodiscard]] constexpr point2d operator-() const { return {-this->x_, -this->y_}; }

    /**
     * @brief Checks if two points are equal.
     * @param other The other point.
     */
    [[nodiscard]] constexpr bool operator==(point2d const& other) const {
      return approx_eq(this->x_, other.x()) and approx_eq(this->y_, other.y());
    }

    /**
     * @brief Checks if two points are not equal.
     * @param other The other point.
     */
    [[nodiscard]] constexpr bool operator!=(point2d const& other) const {
      return not eq(this->x_, other.x()) or not eq(this->y_, other.y());
    }

    /**
     * @brief Sums point and either point2d, size2d or vector2d.
     * @tparam Q The type of the summand. Can be <tt>point2d</tt>, <tt>vector2d</tt> or
     * <tt>size2d</tt>.
     * @param other The summand.
     * @return The sum.
     */
    template <
#ifdef ___rolly_cxx20___
      concepts::any_of<point2d, size2d_type, vector2d_type>
#else
      typename
#endif
        Q>
    [[nodiscard]] constexpr point2d operator+(Q const& other) const {
      return {this->x_ + other.x(), this->y_ + other.y()};
    }

    /**
     * @brief Subtracts point and either point2d, size2d or vector2d.
     * @tparam Q The type of the subtrahend. Can be <tt>point2d</tt>, <tt>vector2d</tt> or
     * <tt>size2d</tt>.
     * @param other The subtrahend.
     * @return The difference.
     */
    template <
#ifdef ___rolly_cxx20___
      concepts::any_of<point2d, size2d_type, vector2d_type>
#else
      typename
#endif
        Q>
    [[nodiscard]] constexpr point2d operator-(Q const& other) const {
      return {this->x_ - other.x(), this->y_ - other.y()};
    }

    /**
     * @brief Multiplies point with scalar.
     * @param other The scalar.
     * @return The product.
     */
    [[nodiscard]] constexpr point2d operator*(number_type const& other) const {
      return {this->x_ * other, this->y_ * other};
    }

    /**
     * @brief Divides point with scalar.
     * @param other The scalar.
     * @return The quotient.
     */
    [[nodiscard]] constexpr point2d operator/(number_type const& other) const {
      return {this->x_ / other, this->y_ / other};
    }

    /**
     * @brief Returns the x-coordinate of the point2d as scalar value.
     * @return The x-coordinate of the point2d.
     */
    [[nodiscard]] constexpr number_type x() const { return this->x_; }

    /**
     * @brief Returns the y-coordinate of the point2d as scalar value.
     * @return The y-coordinate of the point2d.
     */
    [[nodiscard]] constexpr number_type y() const { return this->y_; }

    /**
     * @brief Returns mutable reference to the x-coordinate of the point2d as scalar value.
     * @return Mutable reference to the x-coordinate of the point2d.
     */
    [[nodiscard]] constexpr number_type& x_mut() { return this->x_; }

    /**
     * @brief Returns mutable reference to the y-coordinate of the point2d as scalar value.
     * @return Mutable reference to the y-coordinate of the point2d.
     */
    [[nodiscard]] constexpr number_type& y_mut() { return this->y_; }

    /**
     * @brief Returns inverted point2d.
     * @return Inverted point2d.
     */
    [[nodiscard]] constexpr point2d inverted() const { return {this->y_, this->x_}; }

    /**
     * @brief Adds point and either point2d, size2d or vector2d.
     * @tparam Q The type of the summand. Can be <tt>point2d</tt>, <tt>vector2d</tt> or
     * <tt>size2d</tt>.
     * @param other The summand.
     * @return The sum.
     */
    template <
#ifdef ___rolly_cxx20___
      concepts::any_of<point2d, size2d_type, vector2d_type>
#else
      typename
#endif
        Q>
    constexpr point2d& operator+=(Q const& other) {
      this->x_mut() += other.x();
      this->y_mut() += other.y();
      return *this;
    }

    /**
     * @brief Subtracts point and either point2d, size2d or vector2d.
     * @tparam Q The type of the subtrahend. Can be <tt>point2d</tt>, <tt>vector2d</tt> or
     * <tt>size2d</tt>.
     * @param other The subtrahend.
     * @return The difference.
     */
    template <
#ifdef ___rolly_cxx20___
      concepts::any_of<point2d, size2d_type, vector2d_type>
#else
      typename
#endif
        Q>
    constexpr point2d& operator-=(Q const& other) {
      this->x_mut() -= other.x();
      this->y_mut() -= other.y();
      return *this;
    }

    /**
     * @brief Multiplies point with scalar.
     * @param other The scalar.
     * @return The product.
     */
    constexpr point2d& operator*=(number_type const& other) {
      this->x_mut() *= other;
      this->y_mut() *= other;
      return *this;
    }

    /**
     * @brief Divides point with scalar.
     * @param other The scalar.
     * @return The quotient.
     */
    constexpr point2d& operator/=(number_type const& other) {
      this->x_mut() /= other;
      this->y_mut() /= other;
      return *this;
    }

    /**
     * @brief Constructs new point from size2d.
     * @param other The other size2d.
     */
    [[nodiscard]] static constexpr point2d from_size2d(size2d_type const& other) { return {other}; }

    /**
     * @brief Returns string representation of the point2d.
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
    [[nodiscard]] constexpr point2d round() const {
      return {std::round(this->x_), std::round(this->y_)};
    }

    /**
     * @brief Rounds each component to the smallest integer equal or greater than the original
     * value.
     * @details This behavior is preserved for negative values (unlike the basic cast).
     */
    [[nodiscard]] constexpr point2d ceil() const {
      return {std::ceil(this->x_), std::ceil(this->y_)};
    }

    /**
     * @brief Rounds each component to the largest integer equal or less than the original value.
     * @details This behavior is preserved for negative values (unlike the basic cast).
     */
    [[nodiscard]] constexpr point2d floor() const {
      return {std::floor(this->x_), std::floor(this->y_)};
    }

    /**
     * @brief Linearly interpolate between this point2d and another point2d.
     * @details Example:
     * @code {.cpp}
     * using rolly::point2d;
     * auto const from = point2d(0.0F, 10.0F);
     * auto const to = point2d(8.0F, -4.0F);
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
     * @param other The other point2d.
     * @param t Interpolation factor.
     * @return The interpolated point2d.
     */
    [[nodiscard]] constexpr point2d lerp(point2d const& other, f32 t) const {
      return {this->x_ + ((other.x() - this->x_) * t), this->y_ + ((other.y() - this->y_) * t)};
    }

    /**
     * @brief Returns the largest x and y values of this and another point2d.
     * @param other The other point2d.
     * @return point2d with the largest x and y values.
     */
    [[nodiscard]] constexpr point2d max(point2d const& other) const {
      return {std::max(this->x_, other.x()), std::max(this->y_, other.y())};
    }

    /**
     * @brief Returns the smallest x and y values of this and another point2d.
     * @param other The other point2d.
     * @return point2d with the smallest x and y values.
     */
    [[nodiscard]] constexpr point2d min(point2d const& other) const {
      return {std::min(this->x_, other.x()), std::min(this->y_, other.y())};
    }

    /**
     * @brief Returns the point2d each component of which clamped by corresponding components of
     * start and end.
     * @param start The start point2d.
     * @param end The end point2d.
     * @return The clamped point2d.
     */
    [[nodiscard]] constexpr point2d clamp(point2d const& start, point2d const& end) const {
      return {std::clamp(this->x_, start.x_, end.x_), std::clamp(this->y_, start.y_, end.y_)};
    }

    /**
     * @brief Returns the absolute value of each component.
     * @return The absolute value of each component.
     */
    [[nodiscard]] constexpr point2d abs() const { return {std::abs(this->x_), std::abs(this->y_)}; }

    /**
     * @brief Returns dot product of this and another point2d.
     * @param other The other point2d.
     * @return The dot product of this and another point2d.
     */
    [[nodiscard]] constexpr number_type dot(point2d const& other) const {
      return (this->x_ * other.x()) + (this->y_ * other.y());
    }

    /**
     * @brief Returns the norm of the cross product of this and another point2d.
     * @details Cross product is defined as <code>[x1, y1] x [x2, y2] = x1 * y2 - y1 * x2</code>.
     * @param other The other point2d.
     * @return The norm of the cross product of this and another point2d.
     */
    [[nodiscard]] constexpr number_type cross(point2d const& other) const {
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
    [[nodiscard]] constexpr bool is_positive() const { return this->x_ > 0.0F and this->y_ > 0.0F; }

    /**
     * @brief The same as <i>origin</i>.
     */
    [[nodiscard]] static constexpr point2d zero() { return {}; }

    /**
     * @brief Constructs new point2d, setting all components to the same value.
     * @param value The value to set all components to.
     */
    [[nodiscard]] static constexpr point2d splat(number_type value) { return {value}; }

    /**
     * @brief Constructs new point2d from <tt>std::tuple</tt>.
     * @param other The other <tt>std::tuple</tt>.
     */
#ifdef DOXYGEN
    template <typename... Args>
#else
    template <typename... Args ___sfinae_requirement___(std::tuple_size_v<std::tuple<Args...>> == 2)>
      ___requires___((std::tuple_size_v<std::tuple<Args...>> == 2))
#endif
    [[nodiscard]] static constexpr point2d from_tuple(std::tuple<Args...> const& other) {
      return {std::get<0>(other), std::get<1>(other)};
    }

    /**
     * @brief Constructs new point2d from <tt>std::array</tt>.
     * @tparam N2 Size of the <tt>std::array</tt>. Must be equal to <tt>2</tt>.
     * @param other The other <tt>std::array</tt>.
     */
#ifdef DOXYGEN
    template <std::size_t N2>
      requires(N2 == 2)
#else
    template <std::size_t N2 ___sfinae_requirement___(N2 == 2)> ___requires___((N2 == 2))
#endif
    [[nodiscard]] static constexpr point2d from_array(std::array<number_type, N2> const& other) {
      return {other[0], other[1]};
    }

    /**
     * @brief Constructs new point2d from <tt>std::pair</tt>.
     * @param other The other <tt>std::pair</tt>.
     */
    [[nodiscard]] static constexpr point2d from_pair(
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

#if defined(ROLLY_QT_GUI) || defined(ROLLY_DOC)
    /**
     * @brief Casts this point2d into <tt>QPoint</tt>.
     * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
     * header is compiled for.
     */
    [[nodiscard]] constexpr QPoint to_qpoint() const {
      return QPoint(static_cast<i32>(this->x_), static_cast<i32>(this->y_));
    }

    /**
     * @brief Casts this point2d into <tt>QPointF</tt>.
     * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
     * header is compiled for.
     */
    [[nodiscard]] constexpr QPointF to_qpointf() const { return QPointF(this->x_, this->y_); }

    /**
     * @brief Constructs new point from <tt>QPoint</tt>.
     * @param other The other <tt>QPoint</tt>.
     * @remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this
     * header is compiled for.
     */
    constexpr explicit point2d(QPoint const& other)
      : point2d(other.x(), other.y()) {}

    /**
     * @brief Constructs new point from <tt>QPointF</tt>.
     * @param other The other <tt>QPointF</tt>.
     * @remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this
     * header is compiled for.
     */
    constexpr explicit point2d(QPointF const& other)
      : point2d(other.x(), other.y()) {}

    /**
    * @brief Constructs new point from <tt>QPoint</tt>.
    * @param other The other <tt>QPoint</tt>.
    * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
    header is compiled
    * for.
    /**
    [[nodiscard]] static constexpr point2d from_qpoint(QPoint const& other) { return
    point2d(other.x(), other.y());
    }

    /**
    * @brief Constructs new point from <tt>QPointF</tt>.
    * @param other The other <tt>QPointF</tt>.
    * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
    header is compiled
    * for.
    */
    [[nodiscard]] static constexpr point2d from_qpointf(QPointF const& other) {
      return point2d(other.x(), other.y());
    }
#endif

   private:
    T x_;
    T y_;
  };
}  // namespace rolly

/**
 * @brief Specialization of the <code>fmt::formatter</code> for the @ref rolly::point2d class.
 * @tparam T Number type.
 * @relates rolly::point2d
 */
template <typename T>
struct fmt::formatter<rolly::point2d<T>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  auto format(rolly::point2d<T> const& val, format_context& ctx) const {
    fmt::format_to(ctx.out(), "{}", val.to_string());
    return ctx.out();
  }
};

namespace std {
  /**
   * @brief Hashes a <tt>point2d</tt>.
   * @tparam T Underlying type of the <tt>point2d</tt>.
   * @param b <tt>point2d</tt> to hash.
   * @return Hash value.
   * @relates rolly::point2d
   * @sa http://en.cppreference.com/w/cpp/utility/hash
   */
  template <typename T>
  struct hash<rolly::point2d<T>> {
    size_t operator()(rolly::point2d<T> const& b) const {
      return std::hash<T> {}(b.x()) xor std::hash<T> {}(b.y());
    }
  };
}  // namespace std
