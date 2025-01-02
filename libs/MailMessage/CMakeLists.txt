cmake_minimum_required(VERSION 3.31)
project(MailMessageLib VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(gtest)

set(SOURCES
    src/MailAddress.cpp
    src/MailAttachment.cpp
    src/MailMessageBuilder.cpp
    src/MailMessageFormatter.cpp
)

set(TEST_SOURCES
    test/TestsMailAddress.cpp
    test/TestsMailAttachment.cpp
    test/TestsMailMessageBuilder.cpp
    test/TestsMailMessageFormatter.cpp
)

add_library(MailMessage ${SOURCES})

target_include_directories(MailMessage PUBLIC include)

add_executable(MailMessageTests ${TEST_SOURCES})
target_include_directories(MailMessageTests PUBLIC include)

target_link_libraries(MailMessageTests PRIVATE MailMessage gtest gtest_main)

enable_testing()
add_test(NAME MailMessageTests COMMAND MailMessageTests)