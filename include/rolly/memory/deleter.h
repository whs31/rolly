#pragma once

namespace rolly {
  template <typename R, typename T, R (*F)(T)>
  struct deleter {
    template <typename U>
    void operator()(U& ref) const {
      (*F)(ref);
    }
  };
}  // namespace rolly
