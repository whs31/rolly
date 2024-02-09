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

add_library(${PROJECT_NAME} STATIC)
add_library(sim::kernel ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PUBLIC
    include/simkernel/global.h
    include/simkernel/os.h

    include/simkernel/global/definitions.h
    include/simkernel/global/types.h
    include/simkernel/global/log.h
    include/simkernel/global/logconfig.h
    include/simkernel/global/expected.h
    include/simkernel/global/result.h

    include/simkernel/pattern/iobservable.h
    include/simkernel/pattern/iobserver.h

    include/simkernel/utils/elapsed.h
    include/simkernel/utils/enum.h
    include/simkernel/utils/noexcept.h
    include/simkernel/utils/ranges.h
    include/simkernel/utils/rtti.h
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

install(TARGETS ${PROJECT_NAME})

set(POST_BUILD_COMMENT "[${PROJECT_NAME}] build finished successfully (shared library)")
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ;
  COMMENT "${POST_BUILD_COMMENT}"
)
