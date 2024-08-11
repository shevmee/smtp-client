#include <gtest/gtest.h>
#include "SMTPResponse.h"
#include <stdexcept>

namespace ISXR {

class SMTPResponseTest : public ::testing::Test {
protected:
    SMTPResponseTest() {}
    ~SMTPResponseTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

// Test Case 1: Valid SMTP Response with positive completion (2xx)
TEST_F(SMTPResponseTest, PositiveCompletionResponse) 
{
    SMTPResponse response("250 OK");

    EXPECT_EQ(response.get_code(), 250);
    EXPECT_EQ(response.get_enhanced_code(), "");
    EXPECT_EQ(response.get_text(), "OK");
    EXPECT_EQ(response.get_status(), StatusType::PositiveCompletion);
    EXPECT_TRUE(response.CodeEquals(250));
    EXPECT_TRUE(response.StatusEquals(StatusType::PositiveCompletion));
}

// Test Case 2: Valid SMTP Response with positive intermediate (3xx)
TEST_F(SMTPResponseTest, PositiveIntermediateResponse) 
{
    SMTPResponse response("354 Start mail input");

    EXPECT_EQ(response.get_code(), 354);
    EXPECT_EQ(response.get_enhanced_code(), "");
    EXPECT_EQ(response.get_text(), "Start mail input");
    EXPECT_EQ(response.get_status(), StatusType::PositiveIntermediate);
    EXPECT_TRUE(response.CodeEquals(354));
    EXPECT_TRUE(response.StatusEquals(StatusType::PositiveIntermediate));
}

// Test Case 3: Valid SMTP Response with transient negative completion (4xx)
TEST_F(SMTPResponseTest, TransientNegativeResponse) 
{
    SMTPResponse response("421 Service not available");

    EXPECT_EQ(response.get_code(), 421);
    EXPECT_EQ(response.get_enhanced_code(), "");
    EXPECT_EQ(response.get_text(), "Service not available");
    EXPECT_EQ(response.get_status(), StatusType::TransientNegative);
    EXPECT_TRUE(response.CodeEquals(421));
    EXPECT_TRUE(response.StatusEquals(StatusType::TransientNegative));
}

// Test Case 4: Valid SMTP Response with permanent negative completion (5xx)
TEST_F(SMTPResponseTest, PermanentNegativeResponse) 
{
    SMTPResponse response("550 Requested action not taken");

    EXPECT_EQ(response.get_code(), 550);
    EXPECT_EQ(response.get_enhanced_code(), "");
    EXPECT_EQ(response.get_text(), "Requested action not taken");
    EXPECT_EQ(response.get_status(), StatusType::PermanentNegative);
    EXPECT_TRUE(response.CodeEquals(550));
    EXPECT_TRUE(response.StatusEquals(StatusType::PermanentNegative));
}

// Test Case 5: Valid SMTP Response with enhanced code
TEST_F(SMTPResponseTest, EnhancedCodeResponse) 
{
    SMTPResponse response("250 2.1.5 Recipient OK");

    EXPECT_EQ(response.get_code(), 250);
    EXPECT_EQ(response.get_enhanced_code(), "2.1.5");
    EXPECT_EQ(response.get_text(), "Recipient OK");
    EXPECT_EQ(response.get_status(), StatusType::PositiveCompletion);
    EXPECT_TRUE(response.CodeEquals(250));
    EXPECT_TRUE(response.StatusEquals(StatusType::PositiveCompletion));
}

// Test Case 6: Invalid SMTP Response with non-numeric code
TEST_F(SMTPResponseTest, InvalidResponseFormat) 
{
    EXPECT_THROW(SMTPResponse response("ABC OK"), std::invalid_argument);
}

// Test Case 7: Invalid SMTP Response with incomplete code
TEST_F(SMTPResponseTest, InvalidResponseIncompleteCode) 
{
    EXPECT_THROW(SMTPResponse response("25 OK"), std::invalid_argument);
}

// Test Case 8: Valid SMTP Response with unknown status code
TEST_F(SMTPResponseTest, UnknownStatusCode) 
{
    SMTPResponse response("999 Custom response");

    EXPECT_EQ(response.get_code(), 999);
    EXPECT_EQ(response.get_enhanced_code(), "");
    EXPECT_EQ(response.get_text(), "Custom response");
    EXPECT_EQ(response.get_status(), StatusType::Undefined);
    EXPECT_TRUE(response.CodeEquals(999));
    EXPECT_TRUE(response.StatusEquals(StatusType::Undefined));
}

// Test Case 9: Valid SMTP Response with multiple lines
TEST_F(SMTPResponseTest, MultipleLinesResponse) 
{
    SMTPResponse response("250 1.4.1 First line\r\nSecond line\r\nThird line");

    EXPECT_EQ(response.get_code(), 250);
    EXPECT_EQ(response.get_enhanced_code(), "1.4.1");
    EXPECT_EQ(response.get_text(), "First line\r\nSecond line\r\nThird line");
    EXPECT_EQ(response.get_status(), StatusType::PositiveCompletion);
    EXPECT_TRUE(response.CodeEquals(250));
    EXPECT_TRUE(response.StatusEquals(StatusType::PositiveCompletion));
}

} // namespace ISXR