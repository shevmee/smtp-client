cmake_minimum_required(VERSION 3.10)

project(SmartSocketExample VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system coroutine)

add_subdirectory(libs/Handlers)
add_subdirectory(libs/SMTPResponse)
add_subdirectory(libs/SmartSocket)

add_executable(SmrtExample example.cpp)
target_link_libraries(SmrtExample
    SmartSocket
    SMTPResponse
    Handlers
    OpenSSL::SSL
    ${Boost_LIBRARIES}
)