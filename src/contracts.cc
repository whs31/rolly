#include <cstdio>
#include <cstdlib>
#include <ostream>
#include <rolly/contracts.h>

#include <fmt/color.h>
#include <fmt/format.h>

namespace
{
 using namespace rolly::contracts;
 contract_violation_handler global_violation_handler = default_contract_violation_handler; // NOLINT(*-avoid-non-const-global-variables)
} // namespace

namespace rolly::contracts
{
 void default_contract_violation_handler(contract_violation const& violation) {
   auto type = [&violation]() -> std::string_view {
     switch(violation.type) {
       case contract_type::precondition: return "precondition";
       case contract_type::postcondition: return "postcondition";
       case contract_type::invariant: return "invariant";
       default: return "unknown";
     }
   };
   fmt::print(stderr, fmt::emphasis::bold | fmt::fg(fmt::color::red), "Contract violation ({}):\n", type());
   fmt::print(stderr, fmt::emphasis::bold, "\t{}\n", violation.message);
   fmt::print(stderr, fmt::emphasis::bold, "\tin function '{}'\n", violation.location.function_name());
   fmt::print(stderr, fmt::emphasis::bold, "\tin file '{}'\n", violation.location.file_name());
   fmt::print(stderr, fmt::emphasis::bold, "\tat line {}:{}\n", violation.location.line(), violation.location.column());
   fmt::print(stderr, fmt::emphasis::bold, "\n");
   std::fflush(stderr);
   std::terminate();
 }

 contract_violation_handler& violation_handler() {
   return ::global_violation_handler;
 }

 contract_violation_handler set_violation_handler(contract_violation_handler handler) {
   auto old = ::global_violation_handler;
   ::global_violation_handler = std::move(handler);
   return old;
 }

 contract_violation detail::make_contract_violation(
   contract_type type,
   std::string_view message,
   source_location location
 ) {
   return { type, std::string(message), location };
 }

 void detail::violate(
   contract_type type,
   std::string_view message,
   source_location location
 ) {
   ::violation_handler()(make_contract_violation(type, message, location));
   std::abort();
 }
} // namespace rolly::contracts