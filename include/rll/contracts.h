#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <fmt/format.h>
#include <rll/global/definitions.h>
#include <rll/global/export.h>
#include <rll/source_location.h>

/**
 * @brief Contract-programming related functions and classes.
 * @details Mimics the canceled C++20 contracts proposal.
 *
 * @paragraph Quick introduction
 * Can be used to check preconditions, postconditions and invariants instead of default C-style
 * `assert`:
 * - rll::precondition - these are conditions that
 * must be true before a function (or code segment) executes, in order for the code to work
 * correctly. E.g., a bisection search will work only if the vector is sorted. Another: Converting
 * to polar form will work only if the cartesian coordinates are valid.
 * **It's a good idea to document preconditions.**
 * - rll::postcondition - these are conditions
 * that are true after a functions returns or at the end of a code segment.
 * **Document significant postconditions (i.e., ones that refer to the purpose of the code)**
 * - rll::invariant - these are conditions that is supposed to be true all the
 * time (except for brief, well-defined intervals). E.g., one invariant would be that if the
 * cartesian and polar flags are both true, then the cartesian and polar member variables must
 * represent the same complex number. Another example: in the selection sort, the part of the vector
 * we have already scanned is sorted.
 * **It's a good idea to document significant invariants.**
 *
 * @paragraph Usage
 * You can include contracts.h in your code as follows:
 * @code {.cpp}
 * #include <rll/contracts.h>
 * @endcode
 *
 * @paragraph Examples
 * In first example we use `precondition` to ensure that second argument,
 * passed to function is not zero:
 * @code {.cpp}
 * auto divide(i32 a, i32 b) -> i32 {
 *   assert_precondition(b != 0, "Cannot divide by zero");
 *   return a / b;
 * }
 * @endcode
 *
 * In second example, we ensure that the integer is not overflowed after summing two numbers using
 * `postcondition`:
 * @code {.cpp}
 * auto add(i32 a, i32 b) -> i32 {
 *   auto sum = a + b;
 *   assert_postcondition(sum >= a and sum >= b, "Overflow");
 *   return sum;
 * }
 * @endcode
 *
 * In third example, we the standard stream is open for writing using `invariant`:
 * @code {.cpp}
 * auto println(std::string_view message) -> void {
 *   assert_invariant(std::cout);
 *   std::cout << message << std::endl;
 * }
 * @endcode
 *
 * @note
 * It is a good idea to document both preconditions, postconditions and invariants. Use Doxygen tags
 * `\\pre`, `\\post` and `\\invariant` for this purpose (see http://www.stack.nl/~dimitri/doxygen/).
 */
namespace rll {
  /**
   * @brief Type of contract.
   */
  enum class contract_type : signed char {
    precondition,   ///< Precondition.
    postcondition,  ///< Postcondition.
    invariant       ///< Invariant.
  };

  /**
   * @brief Contract violation data holder.
   * @details Contains information about a contract violation.
   * @see contract_violation_handler
   */
  struct contract_violation {
    contract_type type;        ///< Violated contract type.
    std::string message;       ///< Violation message.
    source_location location;  ///< Violation location in source code.
  };

  /**
   * @brief Contract violation handler type.
   * @note Handler function must be `[[noreturn]]`. The behavior is undefined if it does
   * not terminate the program.
   */
  using contract_violation_handler = std::function<void(contract_violation const&)>;

  /**
   * @brief Default contract violation handler.
   * @details Default implementation of the contract violation handler.
   *
   * Prints the violation to the standard error stream and terminates the program.
   * If it is possible, colors the violation message in red.
   *
   * Example output:
   *
   * @code
   * Contract violation (invariant):
   *    Hello, world!
   *    in function 'int __cdecl main(int,char **)'
   *    in file 'D:/dev/my/floppy-dev/bin/main.cc'
   *    at line 7:18
   *
   * Process finished with exit code 3
   * @endcode
   * @param violation Contract violation data.
   */
  [[noreturn]] RLL_API void default_contract_violation_handler(contract_violation const& violation);

  /**
   * @brief Returns the current global contract violation handler.
   * @return Mutable reference to the global contract violation handler.
   * @see default_contract_violation_handler
   * @see set_violation_handler
   */
  [[nodiscard]] RLL_API contract_violation_handler& violation_handler();

  /**
   * @brief Sets the global contract violation handler and returns the old one.
   * @note Handler function must be <code>[[noreturn]]</code>. The behavior is undefined if it does
   * not terminate the program.
   * @param handler Contract violation handler.
   * @return Old global contract violation handler.
   * @see violation_handler
   * @see default_contract_violation_handler
   */
  RLL_API contract_violation_handler set_violation_handler(contract_violation_handler handler);

  /**
   * @brief Contracts implementation details.
   */
  namespace detail {
    /**
     * @brief Creates a contract violation data structure from current location in the source code.
     * @param type Type of violated contract.
     * @param message Violation message.
     * @param location Violation location in source code. Defaults to current location.
     * @return Contract violation data structure.
     * @see violate
     */
    [[nodiscard]] RLL_API contract_violation make_contract_violation(
      contract_type type,
      std::string_view message,
      source_location location = source_location::current()
    );

    /**
     * @brief Invokes the global contract violation handler (see @ref violation_handler) with the
     * violation data from current location in the source code.
     * @param type Type of violated contract.
     * @param message Violation message.
     * @param location Violation location in source code. Defaults to current location.
     * @see make_contract_violation
     */
    [[noreturn]] RLL_API void violate(
      contract_type type,
      std::string_view message,
      source_location location = source_location::current()
    );
  }  // namespace detail

  /**
   * @brief Checks if a given <i>invariant</i> expression is true and throws a contract violation
   * if it is not.
   * @details If the expression is false, the contract violation is thrown with the given message
   * and location.
   * @note The expression is only evaluated in debug mode. In release mode this function is a
   * no-op.
   * @param expression Invariant expression.
   * @param message Violation message. Defaults to `"Invariant violated"`.
   * @param location Violation location in source code. Defaults to current location.
   * @see broken_invariant
   * @see precondition
   * @see postcondition
   */
  ___inline___ void invariant(
    bool const expression,
    [[maybe_unused]] std::string_view const message = "Invariant violated",
    source_location const& location = source_location::current()
  ) {
    if(not expression)
      detail::violate(contract_type::invariant, message, location);
  }

  /**
   * @brief Checks if a given <i>precondition</i> expression is true and throws a contract
   * violation if it is not.
   * @details If the expression is false, the contract violation is thrown with the given message
   * and location.
   * @note The expression is only evaluated in debug mode. In release mode this function is a
   * no-op.
   * @param expression Precondition expression.
   * @param message Violation message. Defaults to <code>"Precondition violated"</code>.
   * @param location Violation location in source code. Defaults to current location.
   * @see broken_precondition
   * @see invariant
   * @see postcondition
   */
  ___inline___ void precondition(
    bool const expression,
    [[maybe_unused]] std::string_view const message = "Precondition violated",
    source_location const& location = source_location::current()
  ) {
    if(not expression)
      detail::violate(contract_type::precondition, message, location);
  }

  /**
   * @brief Checks if a given <i>postcondition</i> expression is true and throws a contract
   * violation if it is not.
   * @details If the expression is false, the contract violation is thrown with the given message
   * and location.
   * @note The expression is only evaluated in debug mode. In release mode this function is a
   * no-op.
   * @param expression Postcondition expression.
   * @param message Violation message. Defaults to <code>"Postcondition violated"</code>.
   * @param location Violation location in source code. Defaults to current location.
   * @see broken_postcondition
   * @see invariant
   * @see precondition
   */
  ___inline___ void postcondition(
    bool const expression,
    [[maybe_unused]] std::string_view const message = "Postcondition violated",
    source_location const& location = source_location::current()
  ) {
    if(not expression)
      detail::violate(contract_type::postcondition, message, location);
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a
   * broken invariant violation.
   * @details If the expression is false, the contract violation is thrown with the given message
   * and location.
   * @note The expression is only evaluated in debug mode. In release mode this function is a
   * no-op.
   * @param message Violation message. Defaults to <code>"Broken invariant"</code>.
   * @param location Violation location in source code. Defaults to current location.
   * @see invariant
   */
  [[noreturn]] ___inline___ void broken_invariant(
    [[maybe_unused]] std::string_view const message = "Broken invariant",
    source_location const& location = source_location::current()
  ) {
    detail::violate(contract_type::invariant, message, location);
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a
   * broken precondition violation.
   * @param message Violation message. Defaults to <code>"Broken precondition"</code>.
   * @param location Violation location in source code. Defaults to current location.
   * @see precondition
   */
  [[noreturn]] ___inline___ void broken_precondition(
    [[maybe_unused]] std::string_view const message = "Broken precondition",
    source_location const& location = source_location::current()
  ) {
    detail::violate(contract_type::precondition, message, location);
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a
   * broken postcondition violation.
   * @param message Violation message. Defaults to <code>"Broken postcondition"</code>.
   * @param location Violation location in source code. Defaults to current location.
   * @see postcondition
   */
  [[noreturn]] ___inline___ void broken_postcondition(
    [[maybe_unused]] std::string_view const message = "Broken postcondition",
    source_location const& location = source_location::current()
  ) {
    detail::violate(contract_type::postcondition, message, location);
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a
   * broken invariant violation.
   * @param location Violation location in source code. Defaults to current location.
   * @details Useful for marking a function as _unimplemented_.
   * @note The expression is only evaluated in debug mode. In release mode this function is a
   * no-op.
   * @see invariant
   */
  [[noreturn]] ___inline___ void not_implemented(
    source_location const& location = source_location::current()
  ) {
    detail::violate(contract_type::invariant, "Reached unimplemented code", location);
  }
}  // namespace rll

#ifdef RLL_DEBUG
#  define assert_invariant(...) rll::invariant(__VA_ARGS__)
#  define assert_precondition(...) rll::precondition(__VA_ARGS__)
#  define assert_postcondition(...) rll::postcondition(__VA_ARGS__)
#  define assert_broken_invariant(...) rll::broken_invariant(__VA_ARGS__)
#  define assert_broken_precondition(...) rll::broken_precondition(__VA_ARGS__)
#  define assert_not_implemented(...) rll::not_implemented(__VA_ARGS__)
#else
#  define assert_invariant(...)
#  define assert_precondition(...)
#  define assert_postcondition(...)
#  define assert_broken_invariant(...)
#  define assert_broken_precondition(...)
#  define assert_not_implemented(...)
#endif
