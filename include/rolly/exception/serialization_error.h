#pragma once

#include <stdexcept>
#include <fmt/format.h>
#include "../rtti.h"

namespace rolly // NOLINT(*-concat-nested-namespaces)
{
  inline namespace exceptions
  {
    class serialization_error : public std::runtime_error
    {
     public:
      template <typename T>
      serialization_error()
        : std::runtime_error(fmt::format("error during serialization to format \'{}\'", rtti::type_name<T>()))
      {}

      template <typename T>
      explicit serialization_error(std::string_view message)
        : std::runtime_error(fmt::format("error during serialization to format \'{}\': {}", rtti::type_name<T>(), message))
      {}
    };

    class deserialization_error : public std::runtime_error
    {
     public:
      template <typename T>
      deserialization_error()
        : std::runtime_error(fmt::format("error during deserialization from format \'{}\'", rtti::type_name<T>()))
      {}

      template <typename T>
      explicit deserialization_error(std::string_view message)
        : std::runtime_error(fmt::format("error during deserialization from format \'{}\': {}", rtti::type_name<T>(), message))
      {}
    };
  } // namespace exceptions
} // namespace rolly
