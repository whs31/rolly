cmake_minimum_required(VERSION 3.16)

include_guard(GLOBAL)

add_executable(plugin-host main.cc)
target_link_libraries(plugin-host PUBLIC plugin-interface rolly::rolly spdlog::spdlog)

if (WIN32)
  add_custom_command(TARGET plugin-host
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:plugin-host> $<TARGET_FILE_DIR:plugin-host>
    COMMAND_EXPAND_LISTS
  )
endif ()