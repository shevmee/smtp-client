cmake_minimum_required(VERSION 3.0)

project(Handlers VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Add library target
add_library(Handlers STATIC)

# Specify source files
target_sources(Handlers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Handlers.cpp
)

# Include directories for the Handlers target
target_include_directories(Handlers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link the SMTPResponse library to the Handlers target
target_link_libraries(Handlers PUBLIC
    SMTPResponse
)