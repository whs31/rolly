#pragma once

#include <fmt/core.h>

namespace rolly::qt::qml {
  class registrable {
   public:
    virtual ~registrable() = default;

    [[nodiscard]] bool registered() const { return this->registered_; }

    virtual void register_() {
      if(this->registered_) {
        fmt::println("rolly::qt::qml: type is already registered");
        return;
      }
      this->registered_ = true;
    }

   private:
    bool registered_{false};
  };
} // namespace rolly::qt::qml