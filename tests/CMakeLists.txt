find_package(GTest REQUIRED)
find_package(tomlplusplus REQUIRED)
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui QUIET)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui QUIET)

file(GLOB TEST_SOURCES "*.cc")

add_executable(${PROJECT_NAME}-test
  test_guid.cc)
set_target_properties(${PROJECT_NAME}-test PROPERTIES
  CXX_STANDARD ${ROLLY_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_sources(${PROJECT_NAME}-test PRIVATE ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME}-test
  PRIVATE
  GTest::Main
  ${PROJECT_NAME}
  tomlplusplus::tomlplusplus
#  Qt${QT_VERSION_MAJOR}::Core
#  Qt${QT_VERSION_MAJOR}::Gui
)

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME}-test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}-test> $<TARGET_FILE_DIR:${PROJECT_NAME}-test>
    COMMAND_EXPAND_LISTS
  )
endif()
