#pragma once

#include <iostream>
#include "../math.h"
#include "../contracts.h"
#include "../concepts/num.h"
#include "../format/ostream_formatter.h"

namespace rolly {
  /**
   * @brief Measurement unit of the angle.
   */
  enum class angle_unit : char {
    degrees,
    radians
  };

  /**
   * @brief Newtype describing an angle.
   * @details Stores an angle in radians.
   * @tparam T Number type. Must satisfy concept <tt>rolly::concepts::num</tt>. Default is
   * <tt>f32</tt>.
   */
#ifdef DOXYGEN
  template <concepts::num T = f32>
#else
  template <___concept___(concepts::num) T = f32 ___sfinae_requirement___(is_num_v<T>)>
#endif
  struct angle {
    /**
     * @brief Underlying number type.
     */
    using number_type = T;

    /**
     * @brief Constructs an empty angle.
     * @see angle::zero
     */
    constexpr angle()
      : m_(number_type(.0)) {}

    /**
     * @brief Constructs an angle from a numeric value.
     * @param value Numeric value.
     */
    constexpr explicit angle(number_type const value)
      : m_(value) {}

    /**
     * @brief Default destructor.
     */
    ~angle() = default;

    /**
     * @brief Copy constructor.
     */
    constexpr angle(angle const&) = default;

    /**
     * @brief Move constructor.
     */
    constexpr angle(angle&&) = default;

    /**
     * @brief Copy assignment.
     */
    constexpr angle& operator=(angle const&) = default;

    /**
     * @brief Move assignment.
     */
    constexpr angle& operator=(angle&&) = default;

    /**
     * @brief Returns string representation of the angle.
     * @details Angle is represented in degrees.
     * If the underlying number type is floating point, it is rounded to two decimal places.
     * @return String representation of the object.
     */
    [[nodiscard]] std::string to_string() const {
      if constexpr(std::is_floating_point_v<T>)
        return fmt::format("{:.2f}°", this->degrees());
      else
        return fmt::format("{}°", this->degrees());
    }

    /**
     * @brief Returns the angle in the given unit.
     * @param u Unit to return the angle in.
     * @return Angle in the given unit.
     */
    [[nodiscard]] constexpr number_type in(angle_unit u) const {
      switch(u) {
        case angle_unit::radians: return this->radians();
        case angle_unit::degrees: return this->degrees();
        default: contracts::broken_precondition("Unknown unit");
      }
    }

    /**
     * @brief Returns the angle in radians.
     * @see degrees
     */
    [[nodiscard]] constexpr number_type radians() const { return this->m_; }

    /**
     * @brief Returns the angle in degrees.
     * @see radians
     */
    [[nodiscard]] constexpr number_type degrees() const { return to_degrees(this->m_); }

    /**
     * @brief Returns this angle as number in range <tt>[0, 2pi)</tt>.
     */
    [[nodiscard]] constexpr angle positive() const {
      auto const tp = numbers::pi * number_type(2);
      auto a = fmod(this->m_, tp);
      if(a < T(0.0))
        a += tp;
      if(approx_eq(
           a,
           static_cast<T>(tp),
           std::numeric_limits<number_type>::epsilon() * number_type(3)
         ))
        a = T(0.0);
      return angle::from_radians(a);
    }

    /**
     * @brief Returns this angle as number in range <tt>[-pi, pi)</tt>.
     * @return
     */
    [[nodiscard]] constexpr angle signed_() const {
      auto const ret = angle::pi() - (angle::pi() - *this).positive();
      if(std::abs(ret.m_) <= 2.0F * std::numeric_limits<T>::epsilon())
        return angle::zero();
      return ret;
    }

    /**
     * @brief Returns the shortest signed angle between two angles.
     * @param other Other angle
     * @return Takes wrapping and signs into account.
     */
    [[nodiscard]] constexpr angle angle_to(angle const& other) const {
      auto const max = numbers::pi * number_type(2);
      auto const d = fmod(other.radians() - this->radians(), max);
      return angle::from_radians(fmod(number_type(2) * d, max) - d);
    }

    /**
     * @brief Linear interpolation between two angles, using the shortest path.
     * @param other Other angle
     * @param t Interpolation factor
     * @return Interpolated angle
     */
    [[nodiscard]] ___constexpr___ angle lerp(angle const& other, T t) const {
      auto const res = *this + this->angle_to(other) * t;
      if constexpr(std::is_floating_point_v<T>)
        return angle::from_radians(std::round(res.radians() * 1000.0) / 1000.0);
      else
        return res;
    }

    /**
     * @brief Returns the sine and cosine of the angle.
     * @return Pair of sine and cosine.
     */
    [[nodiscard]] constexpr std::pair<T, T> sin_cos() const {
      return std::make_pair(std::sin(this->radians()), std::cos(this->radians()));
    }

    /**
     * @brief Casts the angle to another numeric type.
     * @tparam U Target numeric type.
     * @return Casted angle.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle<U> cast() const {
      return angle<U>(static_cast<U>(this->m_));
    }

    /**
     * @brief Constructs an angle from radians.
     * @param radians Angle in radians.
     * @return Constructed angle.
     * @see from_degrees
     */
    [[nodiscard]] static constexpr angle from_radians(number_type radians) {
      return angle(radians);
    }

    /**
     * @brief Constructs an angle from degrees.
     * @param degrees Angle in degrees.
     * @return Constructed angle.
     * @see from_radians
     */
    [[nodiscard]] static constexpr angle from_degrees(number_type degrees) {
      return angle(to_radians(degrees));
    }

    /**
     * @brief Constructs an angle from another unit.
     * @param value Numeric value.
     * @param u Unit.
     * @return Constructed angle.
     */
    [[nodiscard]] static constexpr angle from(number_type value, angle_unit u) {
      switch(u) {
        case angle_unit::radians: return from_radians(value);
        case angle_unit::degrees: return from_degrees(value);
        default: contracts::broken_precondition("Unknown unit");
      }
    }

    /**
     * @brief Constructs an zero angle.
     */
    [[nodiscard]] static constexpr angle zero() { return angle(static_cast<number_type>(0.0)); }

    /**
     * @brief Constructs an angle with value <tt>pi</tt>.
     */
    [[nodiscard]] static constexpr angle pi() { return angle(numbers::pi); }

    /**
     * @brief Constructs an angle with value <tt>2pi</tt>.
     */
    [[nodiscard]] static constexpr angle two_pi() { return angle(number_type(2.) * numbers::pi); }

    /**
     * @brief Constructs an angle with value <tt>pi/2</tt>.
     */
    [[nodiscard]] static constexpr angle half_pi() { return angle(number_type(.5) * numbers::pi); }

    /**
     * @brief Constructs an angle with value <tt>pi/3</tt>.
     */
    [[nodiscard]] static constexpr angle third_pi() {
      return angle(number_type(.3333333333333) * numbers::pi);
    }

    /**
     * @brief Constructs an angle with value <tt>pi/4</tt>.
     */
    [[nodiscard]] static constexpr angle quarter_pi() {
      return angle(number_type(.25) * numbers::pi);
    }

    /**
     * @brief Returns the underlying numeric scalar value.
     * @returns The underlying numeric scalar value.
     */
    [[nodiscard]] constexpr number_type value() const { return this->m_; }

    /**
     * @brief Returns this numeric newtype as number in another numeric representation.
     * @tparam U Number type. Must satisfy concept <tt>rolly::concepts::num</tt>.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr U as() const {
      return static_cast<U>(this->m_);
    }

    /**
     * @brief Returns this angle as 32-bit floating-point number.
     */
    [[maybe_unused]] [[nodiscard]] constexpr f32 as_f32() const { return this->as<f32>(); }

    /**
     * @brief Returns this angle as 64-bit floating-point number.
     */
    [[maybe_unused]] [[nodiscard]] constexpr f64 as_f64() const { return this->as<f64>(); }

    /**
     * @brief Returns true if this angle is a finite number.
     */
    [[nodiscard]] bool is_finite() const { return std::isfinite(this->m_); }

    /**
     * @brief Returns the sine of this angle.
     */
    [[nodiscard]] ___constexpr___ number_type sin() const { return std::sin(this->m_); }

    /**
     * @brief Returns the cosine of this angle.
     */
    [[nodiscard]] ___constexpr___ number_type cos() const { return std::cos(this->m_); }

    /**
     * @brief Returns the tangent of this angle.
     */
    [[nodiscard]] ___constexpr___ number_type tan() const { return std::tan(this->m_); }

    /**
     * @brief Returns the arc sine of this angle.
     */
    [[nodiscard]] ___constexpr___ number_type asin() const { return std::asin(this->m_); }

    /**
     * @brief Returns the arc cosine of this angle.
     */
    [[nodiscard]] ___constexpr___ number_type acos() const { return std::acos(this->m_); }

    /**
     * @brief Returns the arc tangent of this angle.
     */
    [[nodiscard]] ___constexpr___ number_type atan() const { return std::atan(this->m_); }

    /**
     * @brief Increments the underlying value by 1.
     */
    [[nodiscard]] constexpr T& operator++() {
      ++this->m_;
      return *this;
    }

    /**
     * @brief Increments the underlying value by 1.
     */
    [[nodiscard]] constexpr T& operator++(int) {
      ++this->m_;
      return *this;
    }

    /**
     * @brief Decrements the underlying value by 1.
     */
    [[nodiscard]] constexpr T& operator--() {
      --this->m_;
      return *this;
    }

    /**
     * @brief Decrements the underlying value by 1.
     */
    [[nodiscard]] constexpr T& operator--(int) {
      --this->m_;
      return *this;
    }

    /**
     * @brief Returns <tt>true</tt> if underlying value is not <tt>0</tt>.
     */
    [[nodiscard]] constexpr explicit operator bool() const { return not is_null(this->m_); }

    /**
     * @brief Returns the underlying value as <tt>number_type</tt>.
     */
    [[nodiscard]] constexpr explicit operator number_type() const { return this->m_; }

    /**
     * @brief Compares this and <i>other</i> angle with \ref rolly::math::approx_eq for equality.
     * @param other Other angle.
     * @return <tt>true</tt> if both angles are approximately equal, <tt>false</tt> otherwise.
     */
    [[nodiscard]] constexpr bool operator==(angle const& other) const {
      return approx_eq(
        this->value(),
        other.value(),
        std::numeric_limits<number_type>::epsilon() * number_type(2.)
      );
    }

    /**
     * @brief Compares this and <i>other</i> angle with \ref rolly::math::approx_eq for
     * inequality.
     * @param other Other angle.
     * @return <tt>true</tt> if both angles are not approximately equal, <tt>false</tt> otherwise.
     */
    [[nodiscard]] constexpr bool operator!=(angle const& other) const {
      return not (*this == other);
    }

    /**
     * @brief Compares this and <i>other</i> angle for less than.
     * @param other Other angle.
     * @return <tt>true</tt> if this angle is less than <i>other</i>, <tt>false</tt> otherwise.
     */
    [[nodiscard]] constexpr bool operator<(angle const& other) const {
      return this->value() < other.value();
    }

    /**
     * @brief Compares this and <i>other</i> angle for greater than.
     * @param other Other angle.
     * @return <tt>true</tt> if this angle is greater than <i>other</i>, <tt>false</tt> otherwise.
     */
    [[nodiscard]] constexpr bool operator>(angle const& other) const {
      return this->value() > other.value();
    }

    /**
     * @brief Compares this and <i>other</i> angle for less than or equal to.
     * @param other Other angle.
     * @return <tt>true</tt> if this angle is less than or equal to <i>other</i>, <tt>false</tt>
     * otherwise.
     */
    [[nodiscard]] constexpr bool operator<=(angle const& other) const {
      return not (*this > other);
    }

    /**
     * @brief Compares this and <i>other</i> angle for greater than or equal to.
     * @param other Other angle.
     * @return <tt>true</tt> if this angle is greater than or equal to <i>other</i>,
     * <tt>false</tt> otherwise.
     */
    [[nodiscard]] constexpr bool operator>=(angle const& other) const {
      return not (*this < other);
    }

    /**
     * @brief Returns the underlying numeric scalar.
     * @return The underlying numeric scalar.
     */
    [[nodiscard]] constexpr number_type operator*() const { return this->m_; }

    /**
     * @brief Returns <code>this</code>.
     */
    [[nodiscard]] constexpr angle operator+() const { return *this; }

    /**
     * @brief Returns <tt>-this</tt>.
     */
    [[nodiscard]] constexpr angle operator-() const { return angle(-this->m_); }

    /**
     * @brief Returns <tt>this + other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr angle operator+(angle const& other) const {
      return angle(this->m_ + other.m_);
    }

    /**
     * @brief Returns <tt>this - other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr angle operator-(angle const& other) const {
      return angle(this->m_ - other.m_);
    }

    /**
     * @brief Returns <tt>this * other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr angle operator*(angle const& other) const {
      return angle(this->m_ * other.m_);
    }

    /**
     * @brief Returns <tt>this / other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr angle operator/(angle const& other) const {
      return angle(this->m_ / other.m_);
    }

    /**
     * @brief Returns <tt>this += other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr angle& operator+=(angle const& other) {
      this->m_ += other.m_;
      return *this;
    }

    /**
     * @brief Returns <tt>this -= other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr angle& operator-=(angle const& other) {
      this->m_ -= other.m_;
      return *this;
    }

    /**
     * @brief Returns <tt>this *= other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr angle& operator*=(angle const& other) {
      this->m_ *= other.m_;
      return *this;
    }

    /**
     * @brief Returns <tt>this /= other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr angle& operator/=(angle const& other) {
      this->m_ /= other.m_;
      return *this;
    }

    /**
     * @brief Returns <tt>this * other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle operator*(U const& other) const {
      return angle(this->m_ * other);
    }

    /**
     * @brief Returns <tt>this / other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle operator/(U const& other) const {
      return angle(this->m_ / other);
    }

    /**
     * @brief Returns <tt>this + other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle operator+(U const& other) const {
      return angle(this->m_ + other);
    }

    /**
     * @brief Returns <tt>this - other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle operator-(U const& other) const {
      return angle(this->m_ - other);
    }

    /**
     * @brief Returns <tt>this += other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle& operator+=(U const& other) {
      this->m_ += other;
      return *this;
    }

    /**
     * @brief Returns <tt>this -= other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle& operator-=(U const& other) {
      this->m_ -= other;
      return *this;
    }

    /**
     * @brief Returns <tt>this *= other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle& operator*=(U const& other) {
      this->m_ *= other;
      return *this;
    }

    /**
     * @brief Returns <tt>this /= other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U>
    [[nodiscard]] constexpr angle& operator/=(U const& other) {
      this->m_ /= other;
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, angle const& a) {
      os << a.to_string();
      return os;
    }

   private:
    number_type m_;
  };
}  // namespace rolly

/**
 * @brief Specialization of the <code>fmt::formatter</code> for the @ref rolly::angle class.
 * @tparam T Number type.
 * @relates rolly::angle
 */
template <typename T>
struct fmt::formatter<rolly::angle<T>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  auto format(rolly::angle<T> const& val, format_context& ctx) const {
    fmt::format_to(ctx.out(), "{}", val.to_string());
    return ctx.out();
  }
};
