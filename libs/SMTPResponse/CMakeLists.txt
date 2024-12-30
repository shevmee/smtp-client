cmake_minimum_required(VERSION 3.31)
project(SMTPResponseLib VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(SMTPResponse STATIC
    src/SMTPResponse.cpp
)

target_include_directories(SMTPResponse PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)