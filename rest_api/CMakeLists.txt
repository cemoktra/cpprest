cmake_minimum_required(VERSION 3.5)

project(rest_api VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set (REST_HEADERS
    get_request.hpp
    post_request.hpp
    request.hpp
    request_solver.hpp
    rest_api.hpp
    rest_exceptions.hpp
)

add_library(rest_api INTERFACE)
target_include_directories(rest_api INTERFACE include/)