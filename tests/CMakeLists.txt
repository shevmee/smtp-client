option(BUILD_TESTS "Don't uild the tests" OFF)


if (BUILD_TESTD)
    include(FetchContent)

    FetchContent_Declare(googletest
    	GIT_REPOSITORY https://github.com/google/googletest
    	GIT_TAG release-1.11.0)

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(TestsMain 
        TestsMain.cpp
        TestsMailMessageBuilder.cpp
        TestsMailMessageFormatter.cpp
        TestsMailAttachment.cpp
    )

    target_include_directories(TestsMain PRIVATE 
        ${CMAKE_SOURCE_DIR}/libs/MailMessage/include
    )

    target_link_libraries(TestsMain 
        GTest::gtest_main
        MailMessage
    )

    include(GoogleTest)
    gtest_discover_tests(TestsMain)
endif()