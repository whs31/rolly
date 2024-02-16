cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(leaf
  VERSION 0.2.0
  DESCRIPTION "Kernel library for simulator project and beyond"
  HOMEPAGE_URL "com.radar-mms.uav"
  LANGUAGES C CXX
)

include(GNUInstallDirs)
include(FetchContent)
include(cmake/all.cmake)

set_language_standard(20)
check_language_features()

# On target system the following packages must be present in %PATH%:
  # spdlog
  # fmt
find_package_verbose(spdlog REQUIRED)

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::kernel ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PUBLIC
    include/leaf/global.h
    include/leaf/os.h
    include/leaf/traits.h

    include/leaf/global/definitions.h
    include/leaf/global/types.h
    include/leaf/global/log.h
    include/leaf/global/logconfig.h
    include/leaf/global/expected.h
    include/leaf/global/result.h
    include/leaf/global/sourcelocation.h

    include/leaf/pattern/iobservable.h
    include/leaf/pattern/iobserver.h

    include/leaf/utils/elapsed.h
    include/leaf/utils/enum.h
    include/leaf/utils/noexcept.h
    include/leaf/utils/ranges.h
    include/leaf/utils/rtti.h

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

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src/c++)
target_add_metadata_definitions(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/target/lib)
install(DIRECTORY include/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/target/include)

set(POST_BUILD_COMMENT "[${PROJECT_NAME}] build finished successfully (static library)")
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ;
  COMMENT "${POST_BUILD_COMMENT}"
)
