cmake_minimum_required(VERSION 3.22)
project(battle_city)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME}lib INTERFACE)
target_include_directories(${PROJECT_NAME}lib INTERFACE include)

set_target_properties(${PROJECT_NAME}lib PROPERTIES
        CMAKE_CXX_EXTENSIONS OFF
        LINKER_LANGUAGE CXX
        CMAKE_CXX_STANDARD_REQUIRED ON
        CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp include/magic/vec.h)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}lib)
