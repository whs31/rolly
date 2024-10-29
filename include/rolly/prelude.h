#pragma once

#include "contracts.h"
#include "format.h"
#include "global.h"
#include "log.h"
#include "types/stdint.h"
#include "utility.h"

namespace rolly::prelude {
  namespace contracts = rolly::contracts;
  using namespace rolly::types;
  using namespace rolly::literals;
  using rolly::unreachable;
  using rolly::to_underlying;
  using rolly::logger;
  using rolly::source_location;
} // namespace rolly::prelude