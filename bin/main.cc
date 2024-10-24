#include <rolly/rolly.h>

int main(int /*argc*/, char** /*argv*/) {
  //rolly::contracts::broken_postcondition();
  rolly::contracts::invariant(false, "Hello, world!");
  return 0;
}