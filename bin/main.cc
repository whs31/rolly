#include <rll/all.h>

int main(int /*argc*/, char** /*argv*/) {
  using namespace rll;

  auto ip = "127.0.0.1"_ipv4;
  assert_precondition(ip == net::ipv4_address::parse("127.0.0.1"));
  assert_precondition(ip.is_loopback());
  assert_not_implemented();
  return 0;
}
