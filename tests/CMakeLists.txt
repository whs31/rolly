find_package(Catch2 REQUIRED)
find_package(tomlplusplus REQUIRED)
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui QUIET)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui QUIET)

file(GLOB TEST_SOURCES "*.cc")

add_executable(${PROJECT_NAME}-test)
set_target_properties(${PROJECT_NAME}-test PROPERTIES
  CXX_STANDARD ${ROLLY_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_sources(${PROJECT_NAME}-test PRIVATE ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME}-test
  PRIVATE
    Catch2::Catch2WithMain
    ${PROJECT_NAME}
    tomlplusplus::tomlplusplus
)

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME}-test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}-test> $<TARGET_FILE_DIR:${PROJECT_NAME}-test>
    COMMAND_EXPAND_LISTS
  )
endif()
