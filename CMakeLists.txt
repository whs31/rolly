cmake_minimum_required(VERSION 3.15)

project(simkernel
  VERSION 0.1.0
  DESCRIPTION "Kernel library for simulator project and beyond"
  HOMEPAGE_URL "com.radar-mms.uav"
  LANGUAGES C CXX
)

include(cmake/all.cmake)

pragma_once()

set_language_standard(20)
check_language_features()

# On target system the following packages must be present in %PATH%:
  # spdlog
  # fmt
find_package_verbose(spdlog REQUIRED)

## this is not good and anti-pattern but idgaf im tired of writing million lines of filepaths
#file(GLOB_RECURSE DLM_LIBRARY_SOURCES "src/c++/*.c++" "src/c++/*.h")
#file(GLOB_RECURSE DLM_LIBRARY_INCLUDES "include/dlm/*.h")


add_library(${PROJECT_NAME} STATIC)
add_library(sim::kernel ALIAS ${PROJECT_NAME})

# target_sources(${PROJECT_NAME} PUBLIC ${DLM_LIBRARY_INCLUDES} PRIVATE ${DLM_LIBRARY_SOURCES})

add_subdirectory(libs/expected)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_library(fmt::fmt ALIAS fmt)
endif()

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    fmt
    spdlog::spdlog
    tl::expected
)

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src/c++)
target_add_metadata_definitions(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME})

set(POST_BUILD_COMMENT "[${PROJECT_NAME}] build finished successfully (shared library)")
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ;
  COMMENT "${POST_BUILD_COMMENT}"
)