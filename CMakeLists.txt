cmake_minimum_required(VERSION 3.0)

project(SmtpClient VERSION 1.0)

add_subdirectory(tests)


add_library(MailMessageBuilder 
    ./src/MailMessageBuilder.cpp 
    ./src/MailAddress.cpp
    ./src/MailAttachment.cpp)

target_include_directories(MailMessageBuilder PUBLIC headers)

add_library(MailMessageFormatter 
    ./src/MailMessageFormatter.cpp
    ./src/MailAddress.cpp
    ./src/MailAttachment.cpp
    ./src/MailMessageBuilder.cpp)
    
target_include_directories(MailMessageFormatter PUBLIC headers)



add_executable(main main.cpp)
target_link_libraries(main MailMessageBuilder MailMessageFormatter)