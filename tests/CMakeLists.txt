
# Enable Tests

include(FetchContent)

FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest
	GIT_TAG release-1.11.0)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(TestsMain 
    TestsMain.cpp
    TestsSMTPResponse.cpp
)

target_include_directories(TestsMain PRIVATE 
    ${CMAKE_SOURCE_DIR}/libs/SMTPResponse/include
)

target_link_libraries(TestsMain 
    GTest::gtest_main
    SMTPResponse
)

include(GoogleTest)
gtest_discover_tests(TestsMain)