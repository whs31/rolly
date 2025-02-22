#pragma once

#include <utility>
#include <type_traits>
#include <functional>
#include <rll/types/optional.h>
#include <rll/type_traits.h>

namespace rll {
#ifndef DOXYGEN
  namespace detail {
    template <typename F, typename = std::enable_if_t<std::is_invocable_v<F>>>
    class lazy {
     public:
      using result_type = std::invoke_result_t<F>;

      /**
       * @brief Constructs a lazy object from a given function object.
       * @details The constructor is explicit to avoid accidental copies.
       * @param[in] f Function object to be evaluated when the value is requested the first time.
       * The function object is moved into the lazy object.
       */
      explicit lazy(F&& f)
        : f_(std::move(f)) {}

      /**
       * @brief Constructs a lazy object from a given function object.
       * @details The constructor is explicit to avoid accidental copies.
       * @param[in] f Function object to be evaluated when the value is requested the first time.
       *              The function object is copied into the lazy object.
       */
      explicit lazy(F const& f)
        : f_(f) {}

      ~lazy() = default;
      lazy& operator=(lazy&&) noexcept = delete;

      lazy(lazy&& m) noexcept
        : value_(std::move(m.value_))
        , f_(std::move(m.f_)) {}

      [[nodiscard]] result_type const& operator()() const {
        this->eval();
        return *this->value_;
      }

      [[nodiscard]] result_type& operator()() {
        this->eval();
        return *this->value_;
      }

     private:
      void eval() const {
        if(not this->value_)
          this->value_ = std::invoke(this->f_);
      }

      mutable optional<result_type> value_;
      mutable F f_;
    };
  }  // namespace detail
#endif

  /**
   * @brief Constructs a lazy object from a given function object.
   * @param[in] f Function object to be evaluated when the value is requested the first time. The
   * function object is moved into the lazy object.
   */
  template <typename F>
  [[nodiscard]] auto lazy(F&& f) {
    return detail::lazy<remove_cvref_t<F>>(std::forward<F>(f));
  }
}  // namespace rll
