cmake_minimum_required(VERSION 3.5)

project(rest_apis VERSION 0.1)

# find packages
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Protobuf REQUIRED)

add_subdirectory(argcpp17 EXCLUDE_FROM_ALL)
add_subdirectory(rest_api)
add_subdirectory(tado_api)
add_subdirectory(example)
