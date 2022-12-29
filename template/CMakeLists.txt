cmake_minimum_required(VERSION 3.20)
project(template CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${PROJECT_NAME}_lib INTERFACE)
target_include_directories(${PROJECT_NAME}_lib INTERFACE include)
target_link_libraries(${PROJECT_NAME}_lib INTERFACE ${CONAN_LIBS})
set_target_properties(${PROJECT_NAME}_lib PROPERTIES
        CMAKE_CXX_STANDARD_REQUIRED ON
        CMAKE_CXX_EXTENSIONS OFF
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20)

add_executable(${PROJECT_NAME}_tests
        tests/test.cpp)
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME} src/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20)

enable_testing()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
