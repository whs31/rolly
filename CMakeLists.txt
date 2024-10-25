cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(rolly
  VERSION 2.0.1
  DESCRIPTION "Radar open-source library"
  HOMEPAGE_URL "whs31.github.io/rolly" # todo
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)

include(CMakePrintHelpers)

option(TESTS "Enable integration tests" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ROLLY_COMPAT "Enable C++17 compatibility mode" OFF)

if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(ROLLY_COMPAT)
  set(ROLLY_CXX_STANDARD 17)
else()
  set(ROLLY_CXX_STANDARD 20)
endif()

message(STATUS "[${PROJECT_NAME}] c++ standard: ${ROLLY_CXX_STANDARD}")

# -- dependencies --
find_package(fmt REQUIRED)
if(NOT WIN32)
  find_package(libuuid REQUIRED)
endif()

add_library(${PROJECT_NAME}
  include/rolly/types/guid.h
  src/types/guid.cc
  src/oslayer/android/guid.h
  src/oslayer/android/guid.cc)
add_library(${PROJECT_NAMESPACE}${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD ${ROLLY_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)

cmake_print_properties(TARGETS ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD
  CXX_STANDARD_REQUIRED
  CXX_EXTENSIONS
  POSITION_INDEPENDENT_CODE
)

target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/contracts.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/directories.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtti.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string_util.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/src/types/guid.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/oslayer/linux/dirs.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/oslayer/win/known_folder.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/oslayer/android/guid.cc
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    fmt::fmt
  PRIVATE
    $<$<NOT:$<PLATFORM_ID:Windows>>:libuuid::libuuid>
)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include> PRIVATE src)

if(BUILD_SHARED_LIBS)
  message(STATUS "[${PROJECT_NAME}] adding shared export symbols: ROLLY_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_LIBRARY=1)
else()
  message(STATUS "[${PROJECT_NAME}] adding static export symbols: ROLLY_STATIC_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_STATIC_LIBRARY=1)
endif()

if(ROLLY_COMPAT)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_COMPAT=1)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(STATUS "[${PROJECT_NAME}] adding compiler flags: -Wall -Wextra -Wpedantic -Werror")
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    $<$<NOT:$<CXX_COMPILER_ID:Clang>>:-Werror>
  )
endif()

if(TESTS)
  enable_testing()
  add_subdirectory(tests)
  add_subdirectory(bin)
endif()

# -- installation --
message(STATUS "[${PROJECT_NAME}] compile integration tests status: ${INTEGRATION_TESTS}")
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

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/rolly> $<INSTALL_INTERFACE:include/rolly> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
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
unset(TESTS CACHE)
unset(ROLLY_COMPAT CACHE)
