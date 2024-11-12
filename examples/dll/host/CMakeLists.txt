cmake_minimum_required(VERSION 3.16)

include_guard(GLOBAL)

add_executable(plugin-host main.cc)
target_link_libraries(plugin-host PUBLIC plugin-interface rolly::rolly)
