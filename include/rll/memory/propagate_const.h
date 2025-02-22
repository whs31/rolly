#pragma once

#include <utility>
#include <rll/concepts/ptr.h>

namespace rll {
  template <typename T>
  class propagate_const : public T {
   public:
    using T::operator=;

    propagate_const() = default;

    template <typename... Args>
    inline propagate_const(Args&&... args)  // NOLINT
      : T(std::forward<Args>(args)...) {}

    [[nodiscard]] inline typename T::element_type& operator*() { return T::operator*(); }

    [[nodiscard]] inline typename T::element_type const& operator*() const {
      return T::operator*();
    }

    [[nodiscard]] inline typename T::element_type* operator->() { return T::operator->(); }

    [[nodiscard]] inline typename T::element_type const* operator->() const {
      return T::operator->();
    }

    [[nodiscard]] inline typename T::element_type* get() { return T::get(); }

    [[nodiscard]] inline typename T::element_type const* get() const { return T::get(); }

    [[nodiscard]] inline operator bool() const noexcept {  // NOLINT
      return T::operator bool();
    }
  };
}  // namespace rll
