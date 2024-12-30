cmake_minimum_required(VERSION 3.31)
project(MailMessageLib VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/MailAddress.cpp
    src/MailAttachment.cpp
    src/MailMessageBuilder.cpp
    src/MailMessageFormatter.cpp
)

add_library(MailMessage ${SOURCES})

target_include_directories(MailMessage PUBLIC include)