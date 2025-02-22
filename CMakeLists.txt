cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(rolly
  VERSION 2.4.0
  DESCRIPTION "Radar open-source library"
  HOMEPAGE_URL "whs31.github.io/rolly" # todo
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)
set(PROJECT_FULL_NAME ${PROJECT_NAMESPACE}${PROJECT_NAME})

option(ROLLY_TESTS "Enable integration tests" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ROLLY_QT "Link with Qt" OFF)

if ("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "[${PROJECT_FULL_NAME}] c++ standard: ${ROLLY_CXX_STANDARD}")

# <-- dependencies -->
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(ipaddress REQUIRED)
if (ROLLY_QT)
  message(WARNING "-- [${PROJECT_FULL_NAME}] linking with qt enabled! remember to disable it before push")
  find_package(Qt5
    COMPONENTS
    Core
    Quick
    REQUIRED
  )
  set(CMAKE_AUTOMOC ON)
endif ()
if (NOT WIN32)
  find_package(libuuid REQUIRED)
endif ()

add_library(${PROJECT_NAME})
add_library(${PROJECT_FULL_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)

target_sources(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/contracts.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/directories.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rtti.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/string_util.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/library.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/src/types/guid.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/oslayer/linux/dirs.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/oslayer/win/known_folder.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/oslayer/android/guid.cc
)

if (WIN32)
  target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/oslayer/win/dlfcn_win32.c
  )
else ()
  #  target_sources(${PROJECT_NAME}
  #    PRIVATE
  #
  #  )
endif ()

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  fmt::fmt
  spdlog::spdlog
  ipaddress::ipaddress
  PRIVATE
  $<$<NOT:$<PLATFORM_ID:Windows>>:libuuid::libuuid>
  ${CMAKE_DL_LIBS}
)
if (ROLLY_QT)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
    Qt::Core
    Qt::Quick
    Qt::Qml
  )
endif ()

# <-- msvc-specific options -->
if (MSVC)
  target_include_directories(${PROJECT_NAME} PUBLIC ${FMT_INCLUDE_DIR} ${SPDLOG_INCLUDE_DIR})
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include> PRIVATE src)

if (BUILD_SHARED_LIBS)
  message(STATUS "[${PROJECT_FULL_NAME}] adding shared export symbols: ROLLY_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_LIBRARY=1)
else ()
  message(STATUS "[${PROJECT_FULL_NAME}] adding static export symbols: ROLLY_STATIC_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_STATIC_LIBRARY=1)
endif ()

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:GNU>:-Wno-deprecated-declarations>
  $<$<CXX_COMPILER_ID:Clang>:-Wno-deprecated-declarations>
  $<$<CXX_COMPILER_ID:MSVC>:/wd4996>
)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(STATUS "[${PROJECT_FULL_NAME}] adding compiler flags: -Wall -Wextra -Wpedantic -Werror")
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
  )
endif ()

if (ROLLY_TESTS)
  enable_testing()
  add_subdirectory(tests)
  add_subdirectory(bin)
endif ()

# -- installation --
message(STATUS "[${PROJECT_FULL_NAME}] tests status: ${ROLLY_TESTS}")
message(STATUS "[${PROJECT_FULL_NAME}] installing ${PROJECT_NAME} in namespace ${PROJECT_NAMESPACE}")
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/rll> $<INSTALL_INTERFACE:include/rll> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# -- misc --
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (NOT WIN32)
  message(STATUS "[${PROJECT_FULL_NAME}] copying ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json to the root folder ${CMAKE_CURRENT_SOURCE_DIR}")
  file(CREATE_LINK
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    SYMBOLIC
  )
endif ()

message(STATUS "[${PROJECT_FULL_NAME}] configuring ${PROJECT_NAME} done!")
unset(ROLLY_TESTS CACHE)
unset(ROLLY_QT CACHE)
