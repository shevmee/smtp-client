cmake_minimum_required(VERSION 3.28)
project(HandlersLib VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(Handlers STATIC src/Handlers.cpp)

target_sources(Handlers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Handlers.cpp
)

target_include_directories(Handlers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Handlers PUBLIC
    SMTPResponse
)
