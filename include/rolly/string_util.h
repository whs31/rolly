#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace rolly {
  [[nodiscard]] std::vector<std::string> split(std::string const& input);
  [[nodiscard]] std::vector<std::string> split(std::string_view input);

  [[nodiscard]] std::vector<std::string> split_by(std::string const& input, char delimiter);
  [[nodiscard]] std::vector<std::string> split_by(std::string_view input, char delimiter);

  [[nodiscard]] std::string to_lower(std::string_view input);
}  // namespace rolly
