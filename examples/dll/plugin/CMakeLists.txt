cmake_minimum_required(VERSION 3.16)

add_library(plugin-impl SHARED plugin.cc)
target_link_libraries(plugin-impl PUBLIC plugin-interface rolly::rolly)