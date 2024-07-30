
# Enable Tests

include(FetchContent)

FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest
	GIT_TAG release-1.11.0)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    TestsMailMessageBuilder
    TestsMailMessageBuilder.cpp
    ../src/MailMessageBuilder.cpp
    ../src/MailAttachment.cpp
    ../src/MailAddress.cpp
)

target_include_directories(TestsMailMessageBuilder PRIVATE
    ${CMAKE_SOURCE_DIR}/headers
)

target_link_libraries(TestsMailMessageBuilder gtest_main)


add_executable(
    TestsMailAttachment
    TestsMailAttachment.cpp
    ../src/MailAttachment.cpp
)

target_include_directories(TestsMailAttachment PRIVATE
    ${CMAKE_SOURCE_DIR}/headers
)

target_link_libraries(TestsMailAttachment gtest_main)

add_custom_target(
    runTests
    COMMAND TestsMailMessageBuilder
    COMMAND TestsMailAttachment
    DEPENDS TestsMailMessageBuilder TestsMailAttachment
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests"
)