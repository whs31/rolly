#pragma once

#include <stdexcept>
#include <fmt/format.h>
#include "../rtti.h"
#include "../serialization/tags.h"

namespace rolly  // NOLINT(*-concat-nested-namespaces)
{
  /**
   * @brief Serialization error exception class.
   * @see deserialization_error
   */
  template <typename T = serialization::format::generic>
  class serialization_error : public std::runtime_error {
   public:
    serialization_error()
      : std::runtime_error(
          fmt::format("error during serialization to format \'{}\'", rtti::type_name<T>())
        ) {}

    explicit serialization_error(std::string_view message)
      : std::runtime_error(
          fmt::format(
            "error during serialization to format \'{}\': {}",
            rtti::type_name<T>(),
            message
          )
        ) {}
  };

  /**
   * @brief Deserialization error exception class.
   * @see serialization_error
   */
  template <typename T = serialization::format::generic>
  class deserialization_error : public std::runtime_error {
   public:
    deserialization_error()
      : std::runtime_error(
          fmt::format("error during deserialization from format \'{}\'", rtti::type_name<T>())
        ) {}

    explicit deserialization_error(std::string_view message)
      : std::runtime_error(
          fmt::format(
            "error during deserialization from format \'{}\': {}",
            rtti::type_name<T>(),
            message
          )
        ) {}
  };
}  // namespace rolly
