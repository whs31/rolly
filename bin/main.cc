#include <rolly/rolly.h>

int main(int /*argc*/, char** /*argv*/) {
  // rolly::contracts::broken_postcondition();
  using namespace rolly;

  auto ip = "127.0.0.1"_ipv4;
  contracts::precondition(ip == net::ipv4_address::parse("127.0.0.1"));
  contracts::precondition(ip.is_loopback());
  return 0;
}
