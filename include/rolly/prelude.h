#pragma once

#include "contracts.h"
#include "format.h"
#include "global.h"
#include "log.h"
#include "types/stdint.h"
#include "types/optional.h"
#include "types/result.h"
#include "utility.h"

namespace rolly::prelude {
  namespace contracts = rolly::contracts;
  using namespace rolly::types;
  using namespace rolly::literals;
  using rolly::result;
  using rolly::error;
  using rolly::ok;
  using rolly::optional;
  using rolly::none;
  using rolly::some;
  using rolly::unreachable;
  using rolly::to_underlying;
#ifdef ROLLY_SPDLOG
  using rolly::logger;
#endif  // ROLLY_SPDLOG
  using rolly::source_location;
}  // namespace rolly::prelude
