cmake_minimum_required(VERSION 3.28)

project(HandlersExample)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/Handlers)
add_subdirectory(libs/SMTPResponse)

add_executable(HandlersExample example.cpp)
target_link_libraries(HandlersExample Handlers SMTPResponse)