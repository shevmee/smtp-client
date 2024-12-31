cmake_minimum_required(VERSION 3.31)
project(SMTPResponseLib VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(gtest)

add_library(SMTPResponse STATIC
    src/SMTPResponse.cpp
)

target_include_directories(SMTPResponse PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(SMTPResponseTests test/TestsSMTPResponse.cpp)
target_include_directories(SMTPResponseTests PUBLIC include)

target_link_libraries(SMTPResponseTests PRIVATE SMTPResponse gtest gtest_main)

enable_testing()
add_test(NAME SMTPResponseTests COMMAND SMTPResponseTests)
