cmake_minimum_required(VERSION 3.5)

project(examples VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tado_example tado.cpp)
target_link_libraries(tado_example PUBLIC tado_api argcpp17)

add_executable(github_example github.cpp)
target_link_libraries(github_example PUBLIC github_api argcpp17)