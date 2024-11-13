#pragma once

#include <fmt/core.h>
#include "../types/stdint.h"
#include "../global/version.h"
#include "../contracts.h"
#include "./concepts.h"

namespace rolly::qt::qml {
  enum class verbosity : u8 {
    quiet,
    verbose
  };

#ifdef ROLLY_DEBUG
  inline constexpr verbosity implicit_verbosity {verbosity::verbose};
#else   // defined(ROLLY_DEBUG)
  inline constexpr verbosity implicit_verbosity {verbosity::quiet};
#endif  // defined(ROLLY_DEBUG)

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

    virtual void register_in(module& m) { contracts::not_implemented(); }

   private:
    bool registered_ {false};
  };
}  // namespace rolly::qt::qml
