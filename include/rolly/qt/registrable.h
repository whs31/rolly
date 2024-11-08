#pragma once

#include <fmt/core.h>

namespace rolly::qt::qml {
  class module;

  /**
   * @brief Interface for types that can be registered with QML.
   * @note Only available if Qt::Gui is linked against the project.
   */
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
    
    virtual void register_in(module& m) = 0;

   private:
    bool registered_ {false};
  };
}  // namespace rolly::qt::qml
