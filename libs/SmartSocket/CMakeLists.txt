cmake_minimum_required(VERSION 3.31)
project(SmartSocketLib VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(SmartSocket STATIC src/SmartSocket.cpp)

target_link_libraries(SmartSocket PRIVATE
    Handlers
)

target_include_directories(SmartSocket PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SmartSocket PUBLIC
    SMTPResponse
)