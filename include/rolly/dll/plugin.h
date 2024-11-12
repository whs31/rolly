#pragma once

#include <string>
#include "detail/macro.h"

namespace rolly {  // NOLINT

  /**
   * @brief Dynamic libraries and plugins.
   */
  namespace dll {}
}  // namespace rolly

namespace rolly::dll {
  class plugin {
   public:
    plugin() = default;
    plugin(plugin const&) = default;
    plugin(plugin&&) = default;
    plugin& operator=(plugin const&) = default;
    plugin& operator=(plugin&&) = default;
    virtual ~plugin() = default;

    [[nodiscard]] virtual std::string_view name() const = 0;

    [[nodiscard]] virtual bool init() = 0;
    [[nodiscard]] virtual bool quit() = 0;
  };
}  // namespace rolly::dll
