cmake_minimum_required(VERSION 3.16)

include_guard(GLOBAL)

add_library(plugin-interface INTERFACE include/plugin-api/api.h)
target_link_libraries(plugin-interface INTERFACE rolly::rolly)
target_include_directories(${PROJECT_NAME}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
