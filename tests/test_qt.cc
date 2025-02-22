#include <rll/qt.h>
#include <catch2/catch_all.hpp>

#ifdef RLL_QT_GUI
#  include <test_qt.h>

QtObjectModule::QtObjectModule(QObject* parent) {}

QtGadgetModule::QtGadgetModule() {}

using namespace rll;

TEST_CASE("Qt", "[qt]") {
  SECTION("Qml") {
#  ifdef ___rll_cxx20___
    STATIC_REQUIRE(concepts::qobject<QtObjectModule>);
    STATIC_REQUIRE(concepts::qgadget<QtObjectModule>);
    STATIC_REQUIRE(concepts::qgadget<QtGadgetModule>);
#  endif
    STATIC_REQUIRE(is_qobject_v<QtObjectModule>);
    STATIC_REQUIRE(is_qgadget_v<QtObjectModule>);
    STATIC_REQUIRE(is_qgadget_v<QtGadgetModule>);
  }

  SECTION("Format") {
    REQUIRE(qformat("a={}, b={}", 1, 2) == u"a=1, b=2"_qs);
    REQUIRE(qt::to_std(u"a=1, b=2"_qs) == std::string("a=1, b=2"));
  }
}
#endif
