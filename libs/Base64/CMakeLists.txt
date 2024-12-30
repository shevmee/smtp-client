cmake_minimum_required(VERSION 3.31)
project(Base64Lib VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add library target
add_library(Base64 STATIC src/Base64.cpp)

# Include directories for the SmartSocket target
target_include_directories(Base64 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)