#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <fmt/format.h>
#include "global/definitions.h"
#include "global/export.h"
#include "source_location.h"

/**
 * @brief Contract-programming related functions and classes.
 * @details Mimics the canceled C++20 contracts proposal.
 *
 * @paragraph Quick introduction
 * Can be used to check preconditions, postconditions and invariants instead of default C-style
 * <code>assert</code>: <ul> <li>@ref rolly::contracts::precondition - these are conditions that
 * must be true before a function (or code segment) executes, in order for the code to work
 * correctly. E.g., a bisection search will work only if the vector is sorted. Another: Converting
 * to polar form will work only if the cartesian coordinates are valid. <b>It's a good idea to
 * document preconditions.</b></li> <li>@ref rolly::contracts::postcondition - these are conditions
 * that are true after a functions returns or at the end of a code segment. <b>Document significant
 * postconditions (i.e., ones that refer to the purpose of the code)
 *   .</b></li>
 *   <li>@ref rolly::contracts::invariant - these are conditions that is supposed to be true all the
 * time (except for brief, well-defined intervals). E.g., one invariant would be that if the
 * cartesian and polar flags are both true, then the cartesian and polar member variables must
 * represent the same complex number. Another example: in the selection sort, the part of the vector
 * we have already scanned is sorted. <b>It's a good idea to document significant
 * invariants.</b></li>
 * </ul>
 *
 * @paragraph Usage
 * You can include contracts.h in your code as follows:
 * @code {.cpp}
 * #include <rolly/contracts.h>      // or #include <rolly/prelude.h>
 * using namespace rolly::contracts; // or using namespace rolly::prelude instead
 * @endcode
 *
 * @paragraph Examples
 * In first example we use @ref rolly::contracts::precondition to ensure that second argument,
 * passed to function is not zero:
 * @code {.cpp}
 * auto divide(i32 a, i32 b) -> i32 {
 *   contracts::precondition(b != 0, "Cannot divide by zero");
 *   return a / b;
 * }
 * @endcode
 *
 * In second example, we ensure that the integer is not overflowed after summing two numbers using
 * @ref rolly::contracts::postcondition :
 * @code {.cpp}
 * auto add(i32 a, i32 b) -> i32 {
 *   auto sum = a + b;
 *   contracts::postcondition(sum >= a and sum >= b, "Overflow");
 *   return sum;
 * }
 * @endcode
 *
 * In third example, we the standard stream is open for writing using @ref
 * rolly::contracts::invariant :
 * @code {.cpp}
 * auto println(std::string_view message) -> void {
 *   contracts::invariant(std::cout);
 *   std::cout << message << std::endl;
 * }
 * @endcode
 *
 * @note
 * It is a good idea to document both preconditions, postconditions and invariants. Use Doxygen tags
 * <tt>\\pre</tt>, <tt>\\post</tt> and <tt>\\invariant</tt> for this purpose (see
 * http://www.stack.nl/~dimitri/doxygen/).
 */
namespace rolly::contracts {
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
   * @note Handler function must be <code>[[noreturn]]</code>. The behavior is undefined if it does
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
  [[noreturn]]
#ifndef RLL_DOC
  RLL_API
#endif  // RLL_DOC
    void default_contract_violation_handler(contract_violation const& violation);

  /**
   * @brief Returns the current global contract violation handler.
   * @return Mutable reference to the global contract violation handler.
   * @see default_contract_violation_handler
   * @see set_violation_handler
   */
  [[nodiscard]]
#ifndef RLL_DOC
  RLL_API
#endif  // RLL_DOC
    contract_violation_handler& violation_handler();

/**
 * @brief Sets the global contract violation handler and returns the old one.
 * @note Handler function must be <code>[[noreturn]]</code>. The behavior is undefined if it does
 * not terminate the program.
 * @param handler Contract violation handler.
 * @return Old global contract violation handler.
 * @see violation_handler
 * @see default_contract_violation_handler
 */
#ifndef RLL_DOC
  RLL_API
#endif  // RLL_DOC
  contract_violation_handler set_violation_handler(contract_violation_handler handler);

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
    [[nodiscard]]
#ifndef RLL_DOC
    RLL_API
#endif  // RLL_DOC
      contract_violation make_contract_violation(
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
    [[noreturn]]
#ifndef RLL_DOC
    RLL_API
#endif  // RLL_DOC
      void violate(
        contract_type type,
        std::string_view message,
        source_location location = source_location::current()
      );
  }  // namespace detail

/**
 * @brief Checks if a given <i>invariant</i> expression is true and throws a contract violation if
 * it is not.
 * @details If the expression is false, the contract violation is thrown with the given message and
 * location.
 * @param expression Invariant expression.
 * @param message Violation message. Defaults to <code>"Invariant violated"</code>.
 * @see broken_invariant
 * @see precondition
 * @see postcondition
 */
#ifndef RLL_DOC
  ___inline___
#endif  // RLL_DOC
    void
    invariant(
      bool expression,
      std::string_view message = "Invariant violated",
#ifndef RLL_DOC
      source_location location = source_location::current()
#endif  // RLL_DOC
    ) {
    if(not expression)
      detail::violate(contract_type::invariant, message, location);
  }

/**
 * @brief Checks if a given <i>precondition</i> expression is true and throws a contract violation
 * if it is not.
 * @details If the expression is false, the contract violation is thrown with the given message and
 * location.
 * @param expression Precondition expression.
 * @param message Violation message. Defaults to <code>"Precondition violated"</code>.
 * @param location Violation location in source code. Defaults to current location.
 * @see broken_precondition
 * @see invariant
 * @see postcondition
 */
#ifndef RLL_DOC
  ___inline___
#endif  // RLL_DOC
    void
    precondition(
      bool expression,
      std::string_view message = "Precondition violated",
#ifndef RLL_DOC
      source_location location = source_location::current()
#endif  // RLL_DOC
    ) {
    if(not expression)
      detail::violate(contract_type::precondition, message, location);
  }

/**
 * @brief Checks if a given <i>postcondition</i> expression is true and throws a contract violation
 * if it is not.
 * @details If the expression is false, the contract violation is thrown with the given message and
 * location.
 * @param expression Postcondition expression.
 * @param message Violation message. Defaults to <code>"Postcondition violated"</code>.
 * @see broken_postcondition
 * @see invariant
 * @see precondition
 */
#ifndef RLL_DOC
  ___inline___
#endif  // RLL_DOC
    void
    postcondition(
      bool expression,
      std::string_view message = "Postcondition violated",
#ifndef RLL_DOC
      source_location location = source_location::current()
#endif  // RLL_DOC
    ) {
    if(not expression)
      detail::violate(contract_type::postcondition, message, location);
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken
   * invariant violation.
   * @details If the expression is false, the contract violation is thrown with the given message
   * and location.
   * @param message Violation message. Defaults to <code>"Broken invariant"</code>.
   * @see invariant
   */
  [[noreturn]]
#ifndef RLL_DOC
  ___inline___
#endif  // RLL_DOC
    void broken_invariant(
      std::string_view message = "Broken invariant",
#ifndef RLL_DOC
      source_location location = source_location::current()
#endif  // RLL_DOC
    ) {
    detail::violate(contract_type::invariant, message, location);
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken
   * precondition violation.
   * @param message Violation message. Defaults to <code>"Broken precondition"</code>.
   * @see precondition
   */
  [[noreturn]]
#ifndef RLL_DOC
  ___inline___
#endif  // RLL_DOC
    void broken_precondition(
      std::string_view message = "Broken precondition",
#ifndef RLL_DOC
      source_location location = source_location::current()
#endif  // RLL_DOC
    ) {
    detail::violate(contract_type::precondition, message, location);
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken
   * postcondition violation.
   * @param message Violation message. Defaults to <code>"Broken postcondition"</code>.
   * @see postcondition
   */
  [[noreturn]]
#ifndef RLL_DOC
  ___inline___
#endif  // RLL_DOC
    void broken_postcondition(
      std::string_view message = "Broken postcondition",
#ifndef RLL_DOC
      source_location location = source_location::current()
#endif  // RLL_DOC
    ) {
    detail::violate(contract_type::postcondition, message, location);
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken
   * invariant violation.
   * @details Useful for marking a function as <i>unimplemented</i>.
   * @see invariant
   */
  [[noreturn]]
#ifndef RLL_DOC
  ___inline___
#endif  // RLL_DOC
    void not_implemented(
#ifndef RLL_DOC
      source_location location = source_location::current()
#endif  // RLL_DOC
    ) {
    detail::violate(contract_type::invariant, "Reached unimplemented code", location);
  }

  /**
   * @brief Contracts that will be only executed in debug mode.
   */
  namespace audit {
/**
 * @brief Checks if a given <i>invariant</i> expression is true and throws a contract violation if
 * it is not.
 * @details If the expression is false, the contract violation is thrown with the given message and
 * location.
 * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
 * @param expression Invariant expression.
 * @param message Violation message. Defaults to <code>"Invariant violated"</code>.
 * @see broken_invariant
 * @see precondition
 * @see postcondition
 */
#ifndef RLL_DOC
    ___inline___ ___rolly_release_consteval___
#endif  // RLL_DOC
      void
      invariant(
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
        bool expression,
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
          std::string_view message = "Invariant violated",
#ifndef RLL_DOC
        ___rolly_release_unused___ source_location location = source_location::current()
#endif  // RLL_DOC
      ) {
      ___rolly_debug_only___(if(not expression)
                               detail::violate(contract_type::invariant, message, location));
    }

/**
 * @brief Checks if a given <i>precondition</i> expression is true and throws a contract violation
 * if it is not.
 * @details If the expression is false, the contract violation is thrown with the given message and
 * location.
 * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
 * @param expression Precondition expression.
 * @param message Violation message. Defaults to <code>"Precondition violated"</code>.
 * @param location Violation location in source code. Defaults to current location.
 * @see broken_precondition
 * @see invariant
 * @see postcondition
 */
#ifndef RLL_DOC
    ___inline___ ___rolly_release_consteval___
#endif  // RLL_DOC
      void
      precondition(
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
        bool expression,
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
          std::string_view message = "Precondition violated",
#ifndef RLL_DOC
        ___rolly_release_unused___ source_location location = source_location::current()
#endif  // RLL_DOC
      ) {
      ___rolly_debug_only___(if(not expression)
                               detail::violate(contract_type::precondition, message, location));
    }

/**
 * @brief Checks if a given <i>postcondition</i> expression is true and throws a contract violation
 * if it is not.
 * @details If the expression is false, the contract violation is thrown with the given message and
 * location.
 * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
 * @param expression Postcondition expression.
 * @param message Violation message. Defaults to <code>"Postcondition violated"</code>.
 * @see broken_postcondition
 * @see invariant
 * @see precondition
 */
#ifndef RLL_DOC
    ___inline___ ___rolly_release_consteval___
#endif  // RLL_DOC
      void
      postcondition(
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
        bool expression,
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
          std::string_view message = "Postcondition violated",
#ifndef RLL_DOC
        ___rolly_release_unused___ source_location location = source_location::current()
#endif  // RLL_DOC
      ) {
      ___rolly_debug_only___(if(not expression)
                               detail::violate(contract_type::postcondition, message, location));
    }

/**
 * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken
 * invariant violation.
 * @details If the expression is false, the contract violation is thrown with the given message and
 * location.
 * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
 * @param message Violation message. Defaults to <code>"Broken invariant"</code>.
 * @see invariant
 */
#ifndef RLL_DOC
    [[noreturn]]
    ___inline___ ___rolly_release_consteval___
#endif  // RLL_DOC
      void broken_invariant(
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
          std::string_view message = "Broken invariant",
#ifndef RLL_DOC
        ___rolly_release_unused___ source_location location = source_location::current()
#endif  // RLL_DOC
      ) {
      ___rolly_debug_only___(detail::violate(contract_type::invariant, message, location));
    }

/**
 * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken
 * precondition violation.
 * @param message Violation message. Defaults to <code>"Broken precondition"</code>.
 * @see precondition
 */
#ifndef RLL_DOC
    [[noreturn]]
    ___inline___ ___rolly_release_consteval___
#endif  // RLL_DOC
      void broken_precondition(
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
          std::string_view message = "Broken precondition",
#ifndef RLL_DOC
        ___rolly_release_unused___ source_location location = source_location::current()
#endif  // RLL_DOC
      ) {
      ___rolly_debug_only___(detail::violate(contract_type::precondition, message, location));
    }

/**
 * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken
 * postcondition violation.
 * @param message Violation message. Defaults to <code>"Broken postcondition"</code>.
 * @see postcondition
 */
#ifndef RLL_DOC
    [[noreturn]]
    ___inline___ ___rolly_release_consteval___
#endif  // RLL_DOC
      void broken_postcondition(
#ifndef RLL_DOC
        ___rolly_release_unused___
#endif  // RLL_DOC
          std::string_view message = "Broken postcondition",
#ifndef RLL_DOC
        ___rolly_release_unused___ source_location location = source_location::current()
#endif  // RLL_DOC
      ) {
      ___rolly_debug_only___(detail::violate(contract_type::postcondition, message, location));
    }

    /**
     * @brief Invokes the global contract violation handler (see @ref violation_handler) with a
     * broken invariant violation.
     * @details Useful for marking a function as <i>unimplemented</i>.
     * @note The expression is only evaluated in debug mode. In release mode this function is a
     * no-op.
     * @see invariant
     */
    [[noreturn]]
#ifndef RLL_DOC
    ___inline___ ___rolly_release_consteval___
#endif  // RLL_DOC
      void not_implemented(
#ifndef RLL_DOC
        ___rolly_release_unused___ source_location location = source_location::current()
#endif  // RLL_DOC
      ) {
      ___rolly_debug_only___(
        detail::violate(contract_type::invariant, "Reached unimplemented code", location)
      );
    }
  }  // namespace audit
}  // namespace rolly::contracts
