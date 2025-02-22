#pragma once

#include <string>
#include <array>
#include <tuple>
#include <algorithm>
#include <fmt/format.h>
#include <rll/stdint.h>
#include <rll/concepts/num.h>
#include <rll/concepts/any_of.h>
#include <rll/euclid/size2d.h>
#include <rll/euclid/angle.h>

#if defined(RLL_QT_GUI)
#  include <qvector2d.h>
#endif

namespace rll {
  template <typename T>
  class point2d;

  /**
   * @brief A two-dimensional vector tagged with a unit.
   * @tparam T Number type. Must satisfy concept <tt>rll::concepts::num</tt>. Default is \c f32.
   */
  template <typename T = f32>
  struct vector2d {
    /**
     * @brief Underlying number type.
     */
    using number_type = T;

    /**
     * @brief Associated size2d type.
     */
    using size2d_type = size2d<number_type>;

    /**
     * @brief Associated point2d type.
     */
    using point2d_type = point2d<number_type>;

    /**
     * @brief Associated angle type.
     */
    using angle_type = angle<number_type>;

    /**
     * @brief Constructs new vector with zero coordinates.
     */
    constexpr vector2d()
      : x_(static_cast<number_type>(0))
      , y_(static_cast<number_type>(0)) {}

    /**
     * @brief Constructs new vector from given scalar values.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     */
    constexpr vector2d(number_type x, number_type y)
      : x_(x)
      , y_(y) {}

    /**
     * @brief Constructs new vector, setting all components to the same value.
     * @param value The value to set all components to.
     */
    constexpr explicit vector2d(number_type value)
      : x_(value)
      , y_(value) {}

    /**
     * @brief Constructs new vector from size2d.
     * @param other The other size2d.
     */
    [[nodiscard]] constexpr explicit vector2d(size2d_type const& other)
      : vector2d(other.x(), other.y()) {}

    /**
     * @brief Constructs new vector from point2d.
     * @param other The other point2d.
     */
    [[nodiscard]] constexpr explicit vector2d(point2d<T> const& other)
      : x_(other.x())
      , y_(other.y()) {}

    /**
     * @brief Applies the function <b>fn</b> to each component of the vector2d.
     * @tparam F The type of function to apply.
     * @param fn The function to apply.
     */
    template <typename F, typename = std::enable_if_t<std::is_invocable_v<F>>>
    constexpr auto map(F&& fn) const                            // NOLINT(*-trailing-return)
      -> std::enable_if_t<
        std::is_invocable_v<F, number_type>,
        vector2d<decltype(fn(std::declval<number_type>()))>> {  // NOLINT(*-use-constraints)
      return vector2d<decltype(fn(this->x_))>(fn(this->x_), fn(this->y_));
    }

    /**
     * @brief Applies the function <b>fn</b> to each component of this vector2d and the other
     * vector2d.
     * @tparam F The type of function to apply.
     * @param other The other vector2d to apply.
     * @param fn The function to apply.
     */
    template <
      typename F,
      typename = std::enable_if_t<std::is_invocable_v<F, number_type, number_type>>>
    constexpr auto zip(vector2d const& other, F&& fn) const {
      using result_type = decltype(fn(std::declval<number_type>(), std::declval<number_type>()));
      return vector2d<result_type>(fn(this->x_, other.x()), fn(this->y_, other.y()));
    }

    /**
     * @brief Casts this vector to a size2d.
     * @return The size2d representation of this vector.
     */
    [[nodiscard]] constexpr size2d_type to_size2d() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts this vector to a point2d.
     * @return The point2d representation of this vector.
     */
    [[nodiscard]] constexpr point2d_type to_point2d() const { return {this->x_, this->y_}; }

    /**
     * @brief Returns this vector3d's length squared.
     * @return The length squared of this vector3d.
     */
    [[nodiscard]] constexpr number_type length_squared() const {
      return (this->x_ * this->x_) + (this->y_ * this->y_);
    }

    /**
     * @brief Returns this vector3d's length as scalar value.
     * @return The length of this vector3d.
     */
    [[nodiscard]] constexpr number_type length_scalar() const {
      return number_type(std::sqrt(this->length_squared()));
    }

    /**
     * @brief Returns inverted vector2d.
     * @return Inverted vector2d.
     */
    [[nodiscard]] constexpr vector2d inverted() const { return {this->y_, this->x_}; }

    /**
     * @brief Returns the x-coordinate of the vector2d as scalar value.
     * @return The x-coordinate of the vector2d.
     */
    [[nodiscard]] constexpr number_type x() const { return this->x_; }

    /**
     * @brief Returns the y-coordinate of the vector2d as scalar value.
     * @return The y-coordinate of the vector2d.
     */
    [[nodiscard]] constexpr number_type y() const { return this->y_; }

    /**
     * @brief Returns mutable reference to the x-coordinate of the vector2d as scalar value.
     * @return Mutable reference to the x-coordinate of the vector2d.
     */
    [[nodiscard]] constexpr number_type& x_mut() { return this->x_; }

    /**
     * @brief Returns mutable reference to the y-coordinate of the vector2d as scalar value.
     * @return Mutable reference to the y-coordinate of the vector2d.
     */
    [[nodiscard]] constexpr number_type& y_mut() { return this->y_; }

    /**
     * @brief Returns string representation of the vector2d.
     * @details Size is represented as it's numeric values. If the underlying number type is
     * floating vector2d, it is rounded to two decimal places.
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
    [[nodiscard]] constexpr vector2d round() const {
      return {std::round(this->x_), std::round(this->y_)};
    }

    /**
     * @brief Rounds each component to the smallest integer equal or greater than the original
     * value.
     * @details This behavior is preserved for negative values (unlike the basic cast).
     */
    [[nodiscard]] constexpr vector2d ceil() const {
      return {std::ceil(this->x_), std::ceil(this->y_)};
    }

    /**
     * @brief Rounds each component to the largest integer equal or less than the original value.
     * @details This behavior is preserved for negative values (unlike the basic cast).
     */
    [[nodiscard]] constexpr vector2d floor() const {
      return T(std::floor(this->x_), std::floor(this->y_));
    }

    /**
     * @brief Linearly interpolate between this vector2d and another vector2d.
     * @details Example:
     * @code {.cpp}
     * using rll::vector2d;
     * auto const from = vector2d(0.0F, 10.0F);
     * auto const to = vector2d(8.0F, -4.0F);
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
     * @param other The other vector2d.
     * @param t Interpolation factor.
     * @return The interpolated vector2d.
     */
    [[nodiscard]] constexpr vector2d lerp(vector2d const& other, f32 t) const {
      return {this->x_ + ((other.x() - this->x_) * t), this->y_ + ((other.y() - this->y_) * t)};
    }

    /**
     * @brief Returns the largest x and y values of this and another vector2d.
     * @param other The other vector2d.
     * @return vector2d with the largest x and y values.
     */
    [[nodiscard]] constexpr vector2d max(vector2d const& other) const {
      return {std::max(this->x_, other.x()), std::max(this->y_, other.y())};
    }

    /**
     * @brief Returns the smallest x and y values of this and another vector2d.
     * @param other The other vector2d.
     * @return vector2d with the smallest x and y values.
     */
    [[nodiscard]] constexpr vector2d min(vector2d const& other) const {
      return {std::min(this->x_, other.x()), std::min(this->y_, other.y())};
    }

    /**
     * @brief Returns the vector2d each component of which clamped by corresponding components of
     * start and end.
     * @param start The start vector2d.
     * @param end The end vector2d.
     * @return The clamped vector2d.
     */
    [[nodiscard]] constexpr vector2d clamp(vector2d const& start, vector2d const& end) const {
      return {std::clamp(this->x_, start.x(), end.x()), std::clamp(this->y_, start.y(), end.y())};
    }

    /**
     * @brief Returns dot product of this and another vector2d.
     * @param other The other vector2d.
     * @return The dot product of this and another vector2d.
     */
    [[nodiscard]] constexpr number_type dot(vector2d const& other) const {
      return (this->x_ * other.x()) + (this->y_ * other.y());
    }

    /**
     * @brief Returns the norm of the cross product of this and another vector2d.
     * @details Cross product is defined as <code>[x1, y1] x [x2, y2] = x1 * y2 - y1 * x2</code>.
     * @param other The other vector2d.
     * @return The norm of the cross product of this and another vector2d.
     */
    [[nodiscard]] constexpr number_type cross(vector2d const& other) const {
      return (this->x_ * other.y()) - (this->y_ * other.x());
    }

    /**
     * @brief Returns the absolute value of each component.
     * @return The absolute value of each component.
     */
    [[nodiscard]] constexpr vector2d abs() const {
      return {std::abs(this->x_), std::abs(this->y_)};
    }

    /**
     * @brief Returns this vector projected onto another vector.
     * @param other The other vector.
     * @return The projection of this vector onto the other vector.
     * @warning Projecting onto a zero vector will result in a division by zero.
     */
    [[nodiscard]] constexpr vector2d project(vector2d const& other) const {
      return other * (this->dot(other) / other.length_squared());
    }

    /**
     * @brief Returns the norm of the cross product of this and another vector2d.
     * @details Cross product is defined as <code>[x1, y1] x [x2, y2] = x1 * y2 - y1 * x2</code>.
     * @param other The other vector2d.
     * @return The norm of the cross product of this and another vector2d.
     */
    [[nodiscard]] constexpr number_type cross(T const& other) const {
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
    [[nodiscard]] static constexpr vector2d zero() { return {}; }

    /**
     * @brief Constructs new vector2d, setting all components to the same value.
     * @param value The value to set all components to.
     */
    [[nodiscard]] static constexpr vector2d splat(number_type value) { return {value}; }

    /**
     * @brief Constructs new vector2d from <tt>std::tuple</tt>.
     * @param other The other <tt>std::tuple</tt>.
     */
    template <
      typename... Args,
      typename = std::enable_if_t<std::tuple_size_v<std::tuple<Args...>> == 2>>
    [[nodiscard]] static constexpr vector2d from_tuple(std::tuple<Args...> const& other) {
      return {std::get<0>(other), std::get<1>(other)};
    }

    /**
     * @brief Constructs new vector2d from <tt>std::array</tt>.
     * @tparam N2 The size of the <tt>std::array</tt>. Must be equal to <tt>2</tt>.
     * @param other The other <tt>std::array</tt>.
     */
    template <std::size_t N2, typename = std::enable_if_t<N2 == 2>>
    [[nodiscard]] static constexpr vector2d from_array(std::array<number_type, N2> const& other) {
      return {other[0], other[1]};
    }

    /**
     * @brief Constructs new vector2d from <tt>std::pair</tt>.
     * @param other The other <tt>std::pair</tt>.
     */
    [[nodiscard]] static constexpr vector2d from_pair(
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
     * @brief Returns the signed angle between this vector and another vector.
     * @param other The other vector.
     * @return The signed angle between this vector and the other vector.
     */
    [[nodiscard]] constexpr angle_type angle_to(vector2d const& other) const {
      return angle_type::from_radians(std::atan2(this->cross(other), this->dot(other)));
    }

    /**
     * @brief Returns the signed angle between this vector and the x axis.
     * @details Positive values counted counterclockwise, where 0 is <tt>+x</tt> axis, and
     * <tt>PI/2</tt> is <tt>+y</tt> axis.
     * @return The signed angle between this vector and the x axis.
     */
    [[nodiscard]] constexpr angle_type angle_to_x_axis() const {
      return angle_type::from_radians(std::atan2(this->y_, this->x_));
    }

    /**
     * @brief Returns the vector with length normalized.
     * @return The normalized vector.
     */
    [[nodiscard]] constexpr vector2d normalized() const { return {*this / this->length_scalar()}; }

    /**
     * @brief Returns a reflection vector using an incident ray and a surface normal.
     * @param normal The surface normal.
     * @return The reflection vector.
     */
    [[nodiscard]] constexpr vector2d reflected(vector2d const& normal) const {
      return *this - (normal * number_type(2.0) * this->dot(normal));
    }

    /**
     * @brief Cast from one numeric representation to another, preserving the units.
     * @tparam U New number type.
     * @return The vector2d with the new number type and the same value.
     */
    template <typename U, typename = std::enable_if_t<is_num_v<T>>>
    [[nodiscard]] constexpr vector2d<U> cast() const {
      return {this->x_, this->y_};
    }

    /**
     * @brief Casts into <tt>f32</tt> vector2d.
     */
    [[nodiscard]] constexpr vector2d<> to_f32() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>f64</tt> vector2d.
     */
    [[nodiscard]] constexpr vector2d<f64> to_f64() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>u32</tt> vector2d.
     */
    [[nodiscard]] constexpr vector2d<u32> to_u32() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>u64</tt> vector2d.
     */
    [[nodiscard]] constexpr vector2d<u64> to_u64() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>usize</tt> vector2d.
     */
    [[nodiscard]] constexpr vector2d<usize> to_usize() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>i32</tt> vector2d.
     */
    [[nodiscard]] constexpr vector2d<i32> to_i32() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>i64</tt> vector2d.
     */
    [[nodiscard]] constexpr vector2d<i64> to_i64() const { return {this->x_, this->y_}; }

    /**
     * @brief Casts into <tt>isize</tt> vector2d.
     */
    [[nodiscard]] constexpr vector2d<isize> to_isize() const { return {this->x_, this->y_}; }

    /**
     * @brief Constructs new vector with all values set to one.
     */
    [[nodiscard]] static constexpr vector2d one() { return vector2d::splat(1.0); }

    /**
     * @brief Returns an unchanged copy of this vector.
     * @return An unchanged copy of this vector.
     */
    [[nodiscard]] constexpr vector2d operator+() const { return *this; }

    /**
     * @brief Returns a negated copy of this vector.
     * @return A negated copy of this vector.
     */
    [[nodiscard]] constexpr vector2d operator-() const { return {-this->x_, -this->y_}; }

    /**
     * @brief Returns <tt>true</tt> if the vector is not equal to the other vector.
     * @param other The other vector.
     * @return <tt>true</tt> if the vector is not equal to the other vector.
     */
    [[nodiscard]] constexpr bool operator!=(vector2d const& other) const {
      return not approx_eq(this->x_, other.x(), static_cast<number_type>(3.0F))
          or not approx_eq(this->y_, other.y(), static_cast<number_type>(3.0F));
    }

    /**
     * @brief Adds vector and either point2d, size2d or vector2d.
     * @tparam Q The type of the summand. Can be <tt>point2d</tt>, <tt>vector2d</tt> or
     * <tt>size2d</tt>.
     * @param other The summand.
     * @return The sum.
     */
    template <
#ifdef ___rll_cxx20___
      concepts::any_of<vector2d, size2d_type>
#else
      typename
#endif
        Q>
    [[nodiscard]] constexpr vector2d operator+(Q const& other) const {
      return {this->x_ + other.x(), this->y_ + other.y()};
    }

    /**
     * @brief Returns <tt>true</tt> if the vector is equal to the other vector.
     * @param other The other vector.
     * @return <tt>true</tt> if the vector is equal to the other vector.
     */
    [[nodiscard]] constexpr bool operator==(vector2d const& other) const {
      return approx_eq(this->x_, other.x(), static_cast<number_type>(3.0F))
         and approx_eq(this->y_, other.y(), static_cast<number_type>(3.0F));
    }

    /**
     * @brief Subtracts vector and either point2d, size2d or vector2d.
     * @tparam Q The type of the subtrahend. Can be <tt>point2d</tt>, <tt>vector2d</tt> or
     * <tt>size2d</tt>.
     * @param other The subtrahend.
     * @return The difference.
     */
    template <
#ifdef ___rll_cxx20___
      concepts::any_of<vector2d, size2d_type>
#else
      typename
#endif
        Q>
    [[nodiscard]] constexpr vector2d operator-(Q const& other) const {
      return vector2d(this->x_ - other.x(), this->y_ - other.y());
    }

    /**
     * @brief Multiplies vector by a scalar.
     * @param other The multiplier.
     * @return The result of multiplication.
     */
    [[nodiscard]] constexpr vector2d operator*(number_type const& other) const {
      return vector2d(this->x_ * other, this->y_ * other);
    }

    /**
     * @brief Divides vector by a scalar.
     * @param other The divisor.
     * @return The result of division.
     */
    [[nodiscard]] constexpr vector2d operator/(number_type const& other) const {
      return vector2d(this->x_ / other, this->y_ / other);
    }

    /**
     * @brief Adds vector and either point2d, size2d or vector2d.
     * @tparam Q The type of the summand. Can be <tt>point2d</tt>, <tt>vector2d</tt> or
     * <tt>size2d</tt>.
     * @param other The summand.
     * @return The sum.
     */
    template <
#ifdef ___rll_cxx20___
      concepts::any_of<vector2d, size2d_type>
#else
      typename
#endif
        Q>
    constexpr vector2d& operator+=(Q const& other) {
      this->x_mut() += other.x();
      this->y_mut() += other.y();
      return *this;
    }

    /**
     * @brief Subtracts vector and either point2d, size2d or vector2d.
     * @tparam Q The type of the subtrahend. Can be <tt>point2d</tt>, <tt>vector2d</tt> or
     * <tt>size2d</tt>.
     * @param other The subtrahend.
     * @return The difference.
     */
    template <
#ifdef ___rll_cxx20___
      concepts::any_of<vector2d, size2d_type>
#else
      typename
#endif
        Q>
    constexpr vector2d& operator-=(Q const& other) {
      this->x_mut() -= other.x();
      this->y_mut() -= other.y();
      return *this;
    }

    /**
     * @brief Multiplication assignment operator.
     * @param other Scalar value.
     * @return Reference to this vector.
     */
    constexpr vector2d& operator*=(number_type const& other) {
      this->x_mut() *= other;
      this->y_mut() *= other;
      return *this;
    }

    /**
     * @brief Division assignment operator.
     * @param other Scalar value.
     * @return Reference to this vector.
     */
    constexpr vector2d& operator/=(number_type const& other) {
      this->x_mut() /= other;
      this->y_mut() /= other;
      return *this;
    }

#if defined(RLL_QT_GUI) || defined(RLL_DOC)
    /**
     * @brief Constructs new vector2d from <tt>QVector2D</tt>.
     * @param other The other <tt>QVector2D</tt>.
     * @remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this
     * header is compiled for.
     */
    constexpr explicit vector2d(QVector2D const& other)
      : vector2d(other.x(), other.y()) {}

    /**
     * @brief Constructs new vector2d from <tt>QVector2D</tt>.
     * @param other The other <tt>QVector2D</tt>.
     * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
     * header is compiled for.
     */
    [[nodiscard]] static constexpr vector2d from_qvector2d(QVector2D const& other) {
      return vector2d(other.x(), other.y());
    }

    /**
     * @brief Casts this vector2d into <tt>QVector2D</tt>.
     * @remarks This function is only available if <b>Qt Gui</b> is linked against the TU this
     * header is compiled for.
     */
    [[nodiscard]] constexpr QVector2D to_qvector2d() const { return QVector2D(this->x_, this->y_); }

#endif

   private:
    T x_;
    T y_;
  };
}  // namespace rll

/**
 * @brief Specialization of the `fmt::formatter` for the rll::vector2d class.
 * @tparam T Number type.
 * @relates rll::vector2d
 */
template <typename T>
struct fmt::formatter<rll::vector2d<T>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  auto format(rll::vector2d<T> const& val, format_context& ctx) const {
    fmt::format_to(ctx.out(), "{}", val.to_string());
    return ctx.out();
  }
};

namespace std {
  /**
   * @brief Hashes a <tt>vector2d</tt>.
   * @tparam T Underlying type of the <tt>vector2d</tt>.
   * @param b <tt>vector2d</tt> to hash.
   * @return Hash value.
   * @relates rll::vector2d
   * @sa http://en.cppreference.com/w/cpp/utility/hash
   */
  template <typename T>
  struct hash<rll::vector2d<T>> {
    size_t operator()(rll::vector2d<T> const& b) const {
      return std::hash<T> {}(b.x()) xor std::hash<T> {}(b.y());
    }
  };
}  // namespace std
