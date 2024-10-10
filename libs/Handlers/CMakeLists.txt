cmake_minimum_required(VERSION 3.28)

project(Handlers VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_library(Handlers STATIC)

target_sources(Handlers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Handlers.cpp
)

target_include_directories(Handlers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Handlers PUBLIC
    SMTPResponse
)