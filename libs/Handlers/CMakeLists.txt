cmake_minimum_required(VERSION 3.0)

project(Handlers VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Add library target
add_library(Handlers STATIC)

# Specify source files
target_sources(Handlers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Handlers.cpp
)

# Include directories for the Handlers target
target_include_directories(Handlers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Temporary solution while project is to fully merged
# SCRUM-129/SMTP-response-formatter is stable branch
include(FetchContent)
FetchContent_Declare(smtp_response_lib
  GIT_REPOSITORY "https://github.com/UA-1240-C/smtp-client.git"
  GIT_TAG "origin/SCRUM-129/SMTP-response-formatter"
)
FetchContent_MakeAvailable(smtp_response_lib)

target_link_libraries(Handlers PUBLIC
    SMTPResponse
)