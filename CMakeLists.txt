cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(leaf
  VERSION 0.3.1
  DESCRIPTION "Kernel library for simulator project and beyond"
  HOMEPAGE_URL "com.radar-mms.uav"
  LANGUAGES C CXX
)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(cmake/all.cmake)

set_language_standard(20)
check_language_features()

# On target system the following packages must be present in %PATH%:
  # spdlog
  # fmt
find_package(spdlog REQUIRED CONFIG)

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::leaf ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/global.h> $<INSTALL_INTERFACE:include/leaf/global.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/os.h> $<INSTALL_INTERFACE:include/leaf/os.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf/traits.h> $<INSTALL_INTERFACE:include/leaf/traits.h>

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

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_library(fmt::fmt ALIAS fmt)
endif()

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    fmt
    spdlog::spdlog
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    src/c++
)
target_add_metadata_definitions(${PROJECT_NAME})

# export targets
set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/leafConfig.cmake.in")
set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/leafConfig.cmake")
set(config_targets_file "leafConfigTargets.cmake")
set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/leafConfigVersion.cmake")
set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/leaf")

install(TARGETS leaf
  EXPORT leaf
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/leaf> $<INSTALL_INTERFACE:include/leaf> ;
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
export(TARGETS leaf
  NAMESPACE leaf::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${config_targets_file}"
)
install(EXPORT leaf
  DESTINATION "${export_dest_dir}"
  NAMESPACE leaf::
  FILE "${config_targets_file}"
)
configure_package_config_file(
  "${project_config_in}" "${project_config_out}"
  INSTALL_DESTINATION "${export_dest_dir}"
)
write_basic_package_version_file(
  "${version_config_file}"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)
install(FILES
  "${project_config_out}" "${version_config_file}"
  DESTINATION "${export_dest_dir}"
)
############################################################################

set(POST_BUILD_COMMENT "[${PROJECT_NAME}] build finished successfully (static library)")
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ;
  COMMENT "${POST_BUILD_COMMENT}"
)