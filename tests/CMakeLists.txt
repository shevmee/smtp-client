
# Enable Tests

include(FetchContent)

FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest
	GIT_TAG release-1.11.0)

FetchContent_MakeAvailable(googletest)

enable_testing()

SET(TestsMailMessageBuilderSource 
    TestsMailMessageBuilder.cpp
    ../src/MailMessageBuilder.cpp
    ../src/MailAttachment.cpp
    ../src/MailAddress.cpp
)

SET(TestsMailAttachmentSource 
    TestsMailAttachment.cpp
    ../src/MailAttachment.cpp
)

SET(TestsMailMessageFormatterSource 
    TestsMailMessageFormatter.cpp
    ../src/MailMessageFormatter.cpp
    ../src/MailMessageBuilder.cpp
    ../src/MailAttachment.cpp
    ../src/MailAddress.cpp
)


add_executable(TestsMain 
    TestsMain.cpp
    ${TestsMailMessageBuilderSource}
    ${TestsMailAttachmentSource}
    ${TestsMailMessageFormatterSource}
)

target_include_directories(TestsMain PRIVATE 
    ${CMAKE_SOURCE_DIR}/headers
)

target_link_libraries(TestsMain 
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestsMain)