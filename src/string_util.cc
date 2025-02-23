#include <rll/string_util.h>

#include <algorithm>
#include <iterator>
#include <sstream>

namespace rll {
  std::vector<std::string> split(std::string const& input) {
    auto buffer = std::istringstream(input);
    return {std::istream_iterator<std::string>(buffer), std::istream_iterator<std::string>()};
  }

  std::vector<std::string> split(std::string_view input) {
    auto str = std::string(input);
    return split(str);
  }

  std::vector<std::string> split_by(std::string const& input, char delimiter) {
    auto tokens = std::vector<std::string>();
    auto token = std::string();
    auto token_stream = std::istringstream(input);
    while(std::getline(token_stream, token, delimiter))
      tokens.push_back(token);
    return tokens;
  }

  std::vector<std::string> split_by(std::string_view input, char delimiter) {
    auto str = std::string(input);
    return split_by(str, delimiter);
  }

  std::string to_lower(std::string_view input) {
    auto str = std::string(input);
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
  }
}  // namespace rll
