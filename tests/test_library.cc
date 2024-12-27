#include <rolly/library.h>
#include <catch2/catch_all.hpp>

using namespace rolly;

TEST_CASE("Library") {
  SECTION("Is Library") {
#if defined(ROLLY_OS_WINDOWS)
    REQUIRE(library::is_library("opengl32.dll"));
    REQUIRE(library::is_library("libGL.DLL"));
    REQUIRE(not library::is_library("libGL.so"));
#else
    REQUIRE(library::is_library("libGL.so"));
    REQUIRE(library::is_library("libGL.so.1"));
    REQUIRE(library::is_library("libGL.so.1.2.3"));
    REQUIRE(library::is_library("libGL.0.1.so.3"));
    REQUIRE(not library::is_library("opengl32.dll"));
    REQUIRE(not library::is_library("libGL.DLL"));
#endif
  }  // Is Library
}  // Library
