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

# --- clang-format Integration ---

# Check if clang-format is available
find_program(CLANG_FORMAT_EXECUTABLE clang-format)

# If clang-format is found, create a custom target to run it
if(CLANG_FORMAT_EXECUTABLE)
    # Collect all .cpp and .hpp files recursively using valid paths
    file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp
    )

    # Collect all .cpp and .hpp files in libs subdirectories
    file(GLOB_RECURSE LIB_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/*/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/*/*.hpp
    )

    # Combine both sets of files
    list(APPEND SOURCE_FILES ${LIB_FILES})

    # Add a custom target to run clang-format
    add_custom_target(
        format ALL
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i -style=Google ${SOURCE_FILES}
        COMMENT "Running clang-format on all source files"
        DEPENDS ${SOURCE_FILES}
    )

    # Ensure clang-format runs before the build (this makes it a pre-build step)
    add_dependencies(client format)
else()
    message(WARNING "clang-format not found. Skipping formatting.")
endif()

# --- clang-tidy Integration ---

# Check if clang-tidy is available
# find_program(CLANG_TIDY_EXECUTABLE clang-tidy)

# # If clang-tidy is found, configure it for the client target
# if(CLANG_TIDY_EXECUTABLE)
#     set(CLANG_TIDY_CHECKS "-*,performance-*,readability-*,modernize-*")

#     # Apply clang-tidy to the client target
#     set_target_properties(client PROPERTIES
#         CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE};--checks=${CLANG_TIDY_CHECKS}"
#     )

#     message(STATUS "clang-tidy enabled for the client target with checks: ${CLANG_TIDY_CHECKS}")
# else()
#     message(WARNING "clang-tidy not found. Skipping static analysis.")
# endif()

