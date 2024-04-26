#pragma once

#include <chrono>

namespace leaf::utils {
  struct ElapsedTimer
  {
    ElapsedTimer()
      : start(std::chrono::steady_clock::now())
    {
    }

    [[nodiscard]] auto elapsed() const -> std::chrono::duration<double>
    {
      return std::chrono::steady_clock::now() - start;
    }
    auto reset() -> void { start = std::chrono::steady_clock::now(); }

   private:
    std::chrono::time_point<std::chrono::steady_clock> start;
  };
} // namespace leaf::utils