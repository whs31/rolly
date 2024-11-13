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

  template <typename C>
  bool starts_with(std::basic_string<C> const& input, std::basic_string_view<C> sv) noexcept {
    return sv.size() <= input.size() and std::equal(sv.begin(), sv.end(), input.begin());
  }

  template <typename C>
  bool starts_with(std::basic_string<C> const& input, C c) noexcept {
    return not input.empty() and input[0] == c;
  }

  template <typename C>
  bool starts_with(std::basic_string<C> const& input, C const* chp) noexcept {
    return starts_with(input, std::basic_string_view<C>(chp));
  }

  template <typename C>
  bool ends_with(std::basic_string<C> const& input, std::basic_string_view<C> sv) noexcept {
    return sv.size() <= input.size() and std::equal(sv.rbegin(), sv.rend(), input.rbegin());
  }

  template <typename C>
  bool ends_with(std::basic_string<C> const& input, C c) noexcept {
    return not input.empty() and input[input.size() - 1] == c;
  }

  template <typename C>
  bool ends_with(std::basic_string<C> const& input, C const* chp) noexcept {
    return ends_with(input, std::basic_string_view<C>(chp));
  }
}  // namespace rolly
