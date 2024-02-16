cmake_minimum_required(VERSION 3.15)

project(simkernel
  VERSION 0.2.0
  DESCRIPTION "Kernel library for simulator project and beyond"
  HOMEPAGE_URL "com.radar-mms.uav"
  LANGUAGES C CXX
)

include(GNUInstallDirs)
include(FetchContent)
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
    include/sim/kernel/global.h
    include/sim/kernel/os.h
    include/sim/kernel/traits.h

    include/sim/kernel/global/definitions.h
    include/sim/kernel/global/types.h
    include/sim/kernel/global/log.h
    include/sim/kernel/global/logconfig.h
    include/sim/kernel/global/expected.h
    include/sim/kernel/global/result.h
    include/sim/kernel/global/sourcelocation.h

    include/sim/kernel/pattern/iobservable.h
    include/sim/kernel/pattern/iobserver.h

    include/sim/kernel/utils/elapsed.h
    include/sim/kernel/utils/enum.h
    include/sim/kernel/utils/noexcept.h
    include/sim/kernel/utils/ranges.h
    include/sim/kernel/utils/rtti.h
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

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

set(POST_BUILD_COMMENT "[${PROJECT_NAME}] build finished successfully (static library)")
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ;
  COMMENT "${POST_BUILD_COMMENT}"
)
