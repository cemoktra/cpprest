cmake_minimum_required(VERSION 3.5)

project(github_api VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS github.proto)

set (TADO_SOURCES 
    source/github_api.cpp
)

set (TADO_HEADERS
    include/github_api.hpp
)

add_library(github_api ${TADO_SOURCES} ${TADO_HEADERS} ${PROTO_SRCS} ${PROTO_HDRS})

target_include_directories(github_api
    PUBLIC
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    INTERFACE include/
    PRIVATE include/
)

target_link_libraries(github_api
    PUBLIC
    ${PROTOBUF_LIBRARIES} ${CURL_LIBRARIES} rest_api
)