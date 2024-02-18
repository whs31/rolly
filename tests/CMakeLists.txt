find_package(GTest REQUIRED)
enable_testing()

add_executable(${PROJECT_NAME}-test)

target_sources(${PROJECT_NAME}-test
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/test-main.c++
)

target_link_libraries(${PROJECT_NAME}-test GTest::gtest GTest::gtest_main ${PROJECT_NAME})

