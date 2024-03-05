cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(leaf
  VERSION 0.3.4
  DESCRIPTION "Kernel library for simulator project and beyond"
  HOMEPAGE_URL "io.whs31.leaf"
  LANGUAGES C CXX
)

option(INTEGRATION_TESTS "Enable integration tests" OFF)

include(dependencies/cmake/all.cmake)
enable_cross_compile_options()

set_language_standard(20)
check_language_features()

find_package(spdlog REQUIRED CONFIG)

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::leaf ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global.h> $<INSTALL_INTERFACE:include/leaf/global.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/os.h> $<INSTALL_INTERFACE:include/leaf/os.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/traits.h> $<INSTALL_INTERFACE:include/leaf/traits.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/math.h> $<INSTALL_INTERFACE:include/leaf/math.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/conversion.h> $<INSTALL_INTERFACE:include/leaf/conversion.h>

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global/definitions.h> $<INSTALL_INTERFACE:include/leaf/global/definitions.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global/types.h> $<INSTALL_INTERFACE:include/leaf/global/types.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global/log.h> $<INSTALL_INTERFACE:include/leaf/global/log.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global/logconfig.h> $<INSTALL_INTERFACE:include/leaf/global/logconfig.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global/expected.h> $<INSTALL_INTERFACE:include/leaf/global/expected.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global/result.h> $<INSTALL_INTERFACE:include/leaf/global/result.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global/sourcelocation.h> $<INSTALL_INTERFACE:include/leaf/global/sourcelocation.h>

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/pattern/iobservable.h> $<INSTALL_INTERFACE:include/leaf/pattern/iobservable.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/pattern/iobserver.h> $<INSTALL_INTERFACE:include/leaf/pattern/iobserver.h>

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/utils/elapsed.h> $<INSTALL_INTERFACE:include/leaf/utils/elapsed.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/utils/enum.h> $<INSTALL_INTERFACE:include/leaf/utils/enum.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/utils/noexcept.h> $<INSTALL_INTERFACE:include/leaf/utils/noexcept.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/utils/ranges.h> $<INSTALL_INTERFACE:include/leaf/utils/ranges.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/utils/rtti.h> $<INSTALL_INTERFACE:include/leaf/utils/rtti.h>

  PRIVATE
    src/c++/global/logconfig.c++
    src/c++/utils/rtti.c++
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    #$<$<NOT:$<PLATFORM_ID:Windows>>:fmt>
    spdlog::spdlog
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    src/c++
)

target_add_metadata_definitions(${PROJECT_NAME} COMPANY "whs31")
enable_static_crt()

if(INTEGRATION_TESTS)
  target_add_integration_tests(${PROJECT_NAME}
    TESTS
      tests/test-main.c++
  )
endif()

target_install_library(${PROJECT_NAME} NAME leaf NAMESPACE leaf)

unset(INTEGRATION_TESTS CACHE)
