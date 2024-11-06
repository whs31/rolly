#include <rolly/rolly.h>

int main(int /*argc*/, char** /*argv*/) {
  // rolly::contracts::broken_postcondition();
  using namespace rolly;

  contracts::not_implemented();
  rolly::contracts::invariant(false, "Hello, world!");
  return 0;
}
