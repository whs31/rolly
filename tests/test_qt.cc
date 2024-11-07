#include <rolly/qt/qml.h>
#include <catch2/catch_all.hpp>

#ifdef ROLLY_QT_GUI
#  include <test_qt.h>

QtObjectModule::QtObjectModule(QObject* parent) {}

QtGadgetModule::QtGadgetModule() {}

using namespace rolly;

TEST_CASE("Qt", "[qt]") {
  SECTION("Qml") {
#  ifdef ___rolly_cxx20___
    STATIC_REQUIRE(concepts::qobject<QtObjectModule>);
    STATIC_REQUIRE(concepts::qgadget<QtObjectModule>);
    STATIC_REQUIRE(concepts::qgadget<QtGadgetModule>);
#  endif
    STATIC_REQUIRE(is_qobject_v<QtObjectModule>);
    STATIC_REQUIRE(is_qgadget_v<QtObjectModule>);
    STATIC_REQUIRE(is_qgadget_v<QtGadgetModule>);
  }
}
#endif
