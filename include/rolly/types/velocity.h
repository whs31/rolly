#pragma once

#include <iostream>
#include <fmt/format.h>
#include "../math.h"
#include "../contracts.h"
#include "../concepts/num.h"
#include "./stdint.h"

namespace rolly {
  /**
   * @brief Measurement unit.
   */
  enum class velocity_unit : char {
    kmph = 0,
    mps = 1,
    kilometers_per_hour = kmph,
    meters_per_second = mps
  };

  /**
   * @brief Newtype describing a velocity.
   * @tparam T Number type. Must satisfy concept <tt>rolly::concepts::num</tt>. Default is
   * <tt>f32</tt>.
   */
#ifdef DOXYGEN
  template <concepts::num T = f32>
#else
  template <___concept___(concepts::num) T = f32 ___sfinae_requirement___(is_num_v<T>)>
#endif
  struct velocity {
    /**
     * @brief Underlying number type.
     */
    using number_type = T;

    /**
     * @brief Constructs an zero velocity
     * @see velocity::zero
     */
    constexpr velocity()
      : m_(number_type(.0)) {}

    /**
     * @brief Constructs an velocity from a numeric value.
     * @param value Numeric value.
     */
    constexpr explicit velocity(number_type const value)
      : m_(value) {}

    /**
     * @brief Default destructor.
     */
    ~velocity() = default;

    /**
     * @brief Copy constructor.
     */
    constexpr velocity(velocity const&) = default;

    /**
     * @brief Move constructor.
     */
    constexpr velocity(velocity&&) = default;

    /**
     * @brief Copy assignment.
     */
    constexpr velocity& operator=(velocity const&) = default;

    /**
     * @brief Move assignment.
     */
    constexpr velocity& operator=(velocity&&) = default;

    /**
     * @brief Returns string representation of the velocity.
     * @details Velocity is represented in specified unit.
     * If the underlying number type is floating point, it is rounded to two decimal places.
     * @param u Representation measurement unit. Default is meters per second.
     * @return String representation of the object.
     */
    [[nodiscard]] std::string to_string(velocity_unit u = velocity_unit::meters_per_second) const {
      auto const unit_suffix = [](velocity_unit u) -> std::string_view {
        switch(u) {
          case velocity_unit::kmph: return "km/h";
          case velocity_unit::mps: return "m/s";
          default: contracts::broken_precondition("Unknown velocity unit");
        }
      };
      if constexpr(std::is_floating_point_v<T>)
        return fmt::format("{:.2f} {}", this->in(u), unit_suffix(u));
      else
        return fmt::format("{} {}", this->in(u), unit_suffix(u));
    }

    /**
     * @brief Returns the velocity in the given unit.
     * @param u Unit to return the velocity in.
     * @return Velocity in the given unit.
     */
    [[nodiscard]] constexpr number_type in(velocity_unit u) const {
      switch(u) {
        case velocity_unit::kmph: return this->kmph();
        case velocity_unit::mps: return this->mps();
        default: contracts::broken_precondition("Unknown velocity unit");
      }
    }

    /**
     * @brief Returns the velocity in kilometers per hour.
     * @see mps
     */
    [[nodiscard]] constexpr number_type kmph() const {
      return static_cast<number_type>(static_cast<f64>(this->m_ * 3.6));
    }

    /**
     * @brief Returns the velocity in meters per second.
     * @see kmph
     */
    [[nodiscard]] constexpr number_type mps() const { return this->m_; }

    /**
     * @brief Casts the velocity to another numeric type.
     * @tparam U Target numeric type.
     * @return Casted velocity.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity<U> cast() const {
      return velocity<U>(static_cast<U>(this->m_));
    }

    /**
     * @brief Constructs an velocity from kilometers-per-hour value.
     * @param kmph Velocity in kilometers-per-hour.
     * @return Constructed velocity.
     * @see from_mps
     */
    [[nodiscard]] static constexpr velocity from_kmph(number_type kmph) {
      return velocity(kmph / number_type(3.6));
    }

    /**
     * @brief Constructs an velocity from meters-per-second value.
     * @param mps Velocity in meters-per-second.
     * @return Constructed velocity.
     * @see from_kmph
     */
    [[nodiscard]] static constexpr velocity from_mps(number_type mps) { return velocity(mps); }

    /**
     * @brief Constructs an velocity from another unit.
     * @param value Numeric value.
     * @param u Unit.
     * @return Constructed velocity.
     */
    [[nodiscard]] static constexpr velocity from(number_type value, velocity_unit u) {
      switch(u) {
        case velocity_unit::kmph: return from_kmph(value);
        case velocity_unit::mps: return from_mps(value);
        default: contracts::broken_precondition("Unknown velocity unit");
      }
    }

    /**
     * @brief Constructs an zero velocity.
     */
    [[nodiscard]] static constexpr velocity zero() {
      return velocity(static_cast<number_type>(0.0));
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
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr U as() const {
      return static_cast<U>(this->m_);
    }

    /**
     * @brief Returns this velocity as 32-bit floating-point number.
     */
    [[maybe_unused]] [[nodiscard]] constexpr f32 as_f32() const { return this->as<f32>(); }

    /**
     * @brief Returns this velocity as 64-bit floating-point number.
     */
    [[maybe_unused]] [[nodiscard]] constexpr f64 as_f64() const { return this->as<f64>(); }

    /**
     * @brief Returns true if this velocity is a finite number.
     */
    [[nodiscard]] bool is_finite() const { return std::isfinite(this->m_); }

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
     * @brief Compares this and <i>other</i> velocity with \ref rolly::math::approx_eq for
     * equality.
     * @param other Other velocity.
     * @return <tt>true</tt> if both velocities are approximately equal, <tt>false</tt> otherwise.
     */
    [[nodiscard]] constexpr bool operator==(velocity const& other) const {
      return approx_eq(
        this->value(),
        other.value(),
        std::numeric_limits<number_type>::epsilon() * number_type(2.)
      );
    }

    /**
     * @brief Compares this and <i>other</i> velocity with \ref rolly::math::approx_eq for
     * inequality.
     * @param other Other velocity.
     * @return <tt>true</tt> if both velocities are not approximately equal, <tt>false</tt>
     * otherwise.
     */
    [[nodiscard]] constexpr bool operator!=(velocity const& other) const {
      return not (*this == other);
    }

    /**
     * @brief Compares this and <i>other</i> velocity for less than.
     * @param other Other velocity.
     * @return <tt>true</tt> if this velocities is less than <i>other</i>, <tt>false</tt>
     * otherwise.
     */
    [[nodiscard]] constexpr bool operator<(velocity const& other) const {
      return this->value() < other.value();
    }

    /**
     * @brief Compares this and <i>other</i> velocity for greater than.
     * @param other Other velocity.
     * @return <tt>true</tt> if this velocities is greater than <i>other</i>, <tt>false</tt>
     * otherwise.
     */
    [[nodiscard]] constexpr bool operator>(velocity const& other) const {
      return this->value() > other.value();
    }

    /**
     * @brief Compares this and <i>other</i> velocity for less than or equal to.
     * @param other Other velocity.
     * @return <tt>true</tt> if this velocities is less than or equal to <i>other</i>,
     * <tt>false</tt> otherwise.
     */
    [[nodiscard]] constexpr bool operator<=(velocity const& other) const {
      return not (*this > other);
    }

    /**
     * @brief Compares this and <i>other</i> velocity for greater than or equal to.
     * @param other Other velocity.
     * @return <tt>true</tt> if this velocities is greater than or equal to <i>other</i>,
     * <tt>false</tt> otherwise.
     */
    [[nodiscard]] constexpr bool operator>=(velocity const& other) const {
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
    [[nodiscard]] constexpr velocity operator+() const { return *this; }

    /**
     * @brief Returns <tt>-this</tt>.
     */
    [[nodiscard]] constexpr velocity operator-() const { return T(-this->m_); }

    /**
     * @brief Returns <tt>this + other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr velocity operator+(velocity const& other) const {
      return velocity(this->m_ + other.m_);
    }

    /**
     * @brief Returns <tt>this - other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr velocity operator-(velocity const& other) const {
      return velocity(this->m_ - other.m_);
    }

    /**
     * @brief Returns <tt>this * other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr velocity operator*(velocity const& other) const {
      return velocity(this->m_ * other.m_);
    }

    /**
     * @brief Returns <tt>this / other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr velocity operator/(velocity const& other) const {
      return velocity(this->m_ / other.m_);
    }

    /**
     * @brief Returns <tt>this += other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr velocity& operator+=(velocity const& other) {
      this->m_ += other.m_;
      return *this;
    }

    /**
     * @brief Returns <tt>this -= other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr velocity& operator-=(velocity const& other) {
      this->m_ -= other.m_;
      return *this;
    }

    /**
     * @brief Returns <tt>this *= other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr velocity& operator*=(velocity const& other) {
      this->m_ *= other.m_;
      return *this;
    }

    /**
     * @brief Returns <tt>this /= other</tt>.
     * @param other The other value.
     */
    [[nodiscard]] constexpr velocity& operator/=(velocity const& other) {
      this->m_ /= other.m_;
      return *this;
    }

    /**
     * @brief Returns <tt>this * other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity operator*(U const& other) const {
      return angle(this->m_ * other);
    }

    /**
     * @brief Returns <tt>this / other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity operator/(U const& other) const {
      return angle(this->m_ / other);
    }

    /**
     * @brief Returns <tt>this + other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity operator+(U const& other) const {
      return angle(this->m_ + other);
    }

    /**
     * @brief Returns <tt>this - other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity operator-(U const& other) const {
      return angle(this->m_ - other);
    }

    /**
     * @brief Returns <tt>this += other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity& operator+=(U const& other) {
      this->m_ += other;
      return *this;
    }

    /**
     * @brief Returns <tt>this -= other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity& operator-=(U const& other) {
      this->m_ -= other;
      return *this;
    }

    /**
     * @brief Returns <tt>this *= other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity& operator*=(U const& other) {
      this->m_ *= other;
      return *this;
    }

    /**
     * @brief Returns <tt>this /= other</tt>.
     * @tparam U Right hand side numeric type.
     * @param other The other value.
     */
    template <___concept___(concepts::num) U ___sfinae_requirement___(is_num_v<U>)>
    [[nodiscard]] constexpr velocity& operator/=(U const& other) {
      this->m_ /= other;
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, velocity const& a) {
      os << a.to_string();
      return os;
    }

   private:
    number_type m_;
  };
}  // namespace rolly

/**
 * @brief Specialization of the <code>fmt::formatter</code> for the @ref rolly::velocity class.
 * @tparam T Number type.
 * @relates rolly::velocity
 */
template <typename T>
struct fmt::formatter<rolly::velocity<T>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  auto format(rolly::velocity<T> const& val, format_context& ctx) const {
    fmt::format_to(ctx.out(), "{}", val.to_string());
    return ctx.out();
  }
};
