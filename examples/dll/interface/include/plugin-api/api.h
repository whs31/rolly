#pragma once

#include <rolly/dll/plugin.h>

class ExamplePlugin : public rolly::dll::plugin {
 public:
  [[nodiscard]] virtual std::string hello() const = 0;
};
