cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(leaf
  VERSION 0.5.0
  DESCRIPTION "Coreutils library for C++"
  HOMEPAGE_URL "io.whs31.leaf"
  LANGUAGES C CXX
)

option(INTEGRATION_TESTS "Enable integration tests" OFF)

if(NOT CMAKE_CXX_STANDARD)
  message(STATUS "[${PROJECT_NAME}] setting c++ standard to c++20")
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
message(STATUS "[${PROJECT_NAME}] setting fPIC: ${CMAKE_POSITION_INDEPENDENT_CODE}")

message(STATUS "[${PROJECT_NAME}] checking language features...")
include(CheckCXXSourceCompiles)

message(STATUS "[${PROJECT_NAME}] checking dollar macro...")
check_cxx_source_compiles(
  "#include <iostream>
  #define $dollar(x) x
  int main() {
    std::cout << $dollar(1.0) << std::endl;
    return 0;
  }
  #undef $dollar"
  DOLLAR_MACRO_ALLOWED
)
if(NOT DOLLAR_MACRO_ALLOWED)
  message(FATAL_ERROR "[${PROJECT_NAME}] checking dollar macro... FAILURE")
else()
  message(STATUS "[${PROJECT_NAME}] checking dollar macro... OK!")
endif()

find_package(spdlog REQUIRED CONFIG)
find_package(magic_enum REQUIRED)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::leaf ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global.h> $<INSTALL_INTERFACE:include/leaf/global.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/os.h> $<INSTALL_INTERFACE:include/leaf/os.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/traits.h> $<INSTALL_INTERFACE:include/leaf/traits.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/math.h> $<INSTALL_INTERFACE:include/leaf/math.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/conversion.h> $<INSTALL_INTERFACE:include/leaf/conversion.h>

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/logger.h> $<INSTALL_INTERFACE:include/leaf/logger.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/leaf.h> $<INSTALL_INTERFACE:include/leaf/leaf.h>

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
    src/c++/logger.c++
    src/c++/utils/rtti.c++
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    spdlog::spdlog
  PRIVATE
    magic_enum::magic_enum
)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include> PRIVATE src/c++)

if(BUILD_SHARED_LIBS)
  message(STATUS "[${PROJECT_NAME}] adding shared export symbols: LEAF_LIBRARY")
  set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DLEAF_LIBRARY=1)
else()
  message(STATUS "[${PROJECT_NAME}] adding static export symbols: LEAF_STATIC_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DLEAF_STATIC_LIBRARY=1)
endif()

message(STATUS "[${PROJECT_NAME}] setting metadata definitions:")
message(STATUS "[${PROJECT_NAME}] - PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "[${PROJECT_NAME}] - PROJECT_VERSION: ${PROJECT_VERSION}")
message(STATUS "[${PROJECT_NAME}] - PROJECT_COMPANY: whs31")
message(STATUS "[${PROJECT_NAME}] - PROJECT_DOMAIN: ${PROJECT_HOMEPAGE_URL}")

message(STATUS "[${PROJECT_NAME}] compile integration tests status: ${INTEGRATION_TESTS}")
if(INTEGRATION_TESTS)
  find_package(GTest REQUIRED)
  enable_testing()
  target_sources(${PROJECT_NAME}-test PRIVATE tests/test-main.c++)

  target_link_libraries(${PROJECT_NAME}-test
    PRIVATE
      ${PROJECT_NAME}
      GTest::GTest
      GTest::Main
  )
  install(TARGETS ${PROJECT_NAME}-test
    EXPORT ${PROJECT_NAME}-test
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endif()

set(PROJECT_NAMESPACE leaf::)
message(STATUS "[${PROJECT_NAME}] installing ${PROJECT_NAME} in namespace ${PROJECT_NAMESPACE}")
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
message(STATUS "[${PROJECT_NAME}] installing in prefix: ${CMAKE_INSTALL_PREFIX}")

set(PROJECT_CONFIG_FILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in")
set(PROJECT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
set(CONFIG_TARGETS_FILE "${PROJECT_NAME}-config-targets.cmake")
set(CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
set(INSTALL_EXPORT_DESTINATION_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf> $<INSTALL_INTERFACE:include/leaf> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
export(TARGETS ${PROJECT_NAME} NAMESPACE ${PROJECT_NAMESPACE} FILE "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TARGETS_FILE}")
install(EXPORT ${PROJECT_NAME}
  DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR}
  NAMESPACE ${PROJECT_NAMESPACE}
  FILE ${CONFIG_TARGETS_FILE}
)
configure_package_config_file(${PROJECT_CONFIG_FILE_IN} ${PROJECT_CONFIG_FILE} INSTALL_DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})
write_basic_package_version_file(${CONFIG_VERSION_FILE} COMPATIBILITY AnyNewerVersion)
install(FILES ${PROJECT_CONFIG_FILE} ${CONFIG_VERSION_FILE} DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})

message(STATUS "[${PROJECT_NAME}] configuring ${PROJECT_NAME} done!")

unset(INTEGRATION_TESTS CACHE)
