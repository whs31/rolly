#pragma once

#include <stdexcept>
#include <string_view>
#include <fmt/format.h>

namespace rolly {
  /**
   * @brief Compile-time error exception class.
   */
  class compile_time_error : public std::exception {
    std::string message_;

   public:
    /**
     * @brief Construct a new compile time error object with a default message.
     * @details The default message is "compile-time error occurred."
     */
    compile_time_error()
      : message_("compile-time error occurred.\n") {}

    /**
     * @brief Construct a new compile time error object with a message.
     * @param message Error message.
     */
    explicit compile_time_error(std::string_view message)
      : message_(fmt::format("compile-time error: {}\n", message)) {}

    /**
     * @brief Construct a new compile time error object with a message and format arguments.
     * @details The format arguments are forwarded to the <code>fmt::format()</code> function.
     * @tparam Args Format arguments variadic pack.
     * @param message Error message.
     * @param args Format arguments.
     */
    template <typename... Args>
    explicit compile_time_error(std::string_view message, Args&&... args)
      : message_(
          fmt::format("compile-time error: {}\n", fmt::format(message, std::forward<Args>(args)...))
            .c_str()
        ) {}

    ~compile_time_error() noexcept override = default;
    compile_time_error(compile_time_error const&) = default;
    compile_time_error(compile_time_error&&) = default;
    compile_time_error& operator=(compile_time_error const&) = default;
    compile_time_error& operator=(compile_time_error&&) = default;

    /**
     * @brief Returns the error message.
     */
    [[nodiscard]] char const* what() const noexcept override { return this->message_.c_str(); }
  };
}  // namespace rolly
