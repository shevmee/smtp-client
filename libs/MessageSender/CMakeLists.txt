cmake_minimum_required(VERSION 3.31)
project(MailMessageSenderLib VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(MessageSender
    src/MessageSender.cpp
)

target_include_directories(MessageSender PUBLIC include)

target_link_libraries(MessageSender
    MailMessage
    Base64
)