cmake_minimum_required(VERSION 3.31)
cmake_policy(SET CMP0167 NEW)

project(SmtpClient VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Find Boost package with all required components
find_package(Boost 1.83 REQUIRED COMPONENTS system coroutine)

# Ensure pthread is found if it's needed (Boost with threading requires it)
find_package(Threads REQUIRED)

# Add executable target
add_executable(client)

# Specify source files
target_sources(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SmtpClient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Include directories for the client target
target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# Add subdirectories for libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/Handlers)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/Base64)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SmartSocket)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SMTPResponse)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/MailMessage)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/MessageSender)

# Add subdirectory for tests
add_subdirectory(tests)

# Link libraries to the client target
target_link_libraries(client PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    SmartSocket
    Base64
    ${Boost_LIBRARIES}       # Includes Boost system and coroutine
    SMTPResponse
    MailMessage
    MessageSender
    Threads::Threads         # Link pthreads (required for Boost components with threading)
)
