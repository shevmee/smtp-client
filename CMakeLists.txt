cmake_minimum_required(VERSION 3.0)

project(Response)

add_subdirectory(libs/SMTPResponse)

add_executable(Response main.cpp)

target_link_libraries(Response SMTPResponse)


