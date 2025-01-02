#include <gtest/gtest.h>

#include <string>

#include "SMTPResponse.hpp"

using namespace ISXResponse;

// Test Case 1: Valid SMTP Response with positive completion (2xx)
TEST(SMTPResponseTest, PositiveCompletionResponse) {
  auto result = SMTPResponse::Create("250 OK");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_code(), 250);
  EXPECT_EQ(response.get_enhanced_code(), "");
  EXPECT_EQ(response.get_text(), "OK");
  EXPECT_EQ(response.get_status(), StatusType::PositiveCompletion);
  EXPECT_TRUE(response.CodeEquals(250));
  EXPECT_TRUE(response.StatusEquals(StatusType::PositiveCompletion));
}

// Test Case 2: Valid SMTP Response with positive intermediate (3xx)
TEST(SMTPResponseTest, PositiveIntermediateResponse) {
  auto result = SMTPResponse::Create("354 Start mail input");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_code(), 354);
  EXPECT_EQ(response.get_enhanced_code(), "");
  EXPECT_EQ(response.get_text(), "Start mail input");
  EXPECT_EQ(response.get_status(), StatusType::PositiveIntermediate);
  EXPECT_TRUE(response.CodeEquals(354));
  EXPECT_TRUE(response.StatusEquals(StatusType::PositiveIntermediate));
}

// Test Case 3: Valid SMTP Response with transient negative completion (4xx)
TEST(SMTPResponseTest, TransientNegativeResponse) {
  auto result = SMTPResponse::Create("421 Service not available");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_code(), 421);
  EXPECT_EQ(response.get_enhanced_code(), "");
  EXPECT_EQ(response.get_text(), "Service not available");
  EXPECT_EQ(response.get_status(), StatusType::TransientNegative);
  EXPECT_TRUE(response.CodeEquals(421));
  EXPECT_TRUE(response.StatusEquals(StatusType::TransientNegative));
}

// Test Case 4: Valid SMTP Response with permanent negative completion (5xx)
TEST(SMTPResponseTest, PermanentNegativeResponse) {
  auto result = SMTPResponse::Create("550 Requested action not taken");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_code(), 550);
  EXPECT_EQ(response.get_enhanced_code(), "");
  EXPECT_EQ(response.get_text(), "Requested action not taken");
  EXPECT_EQ(response.get_status(), StatusType::PermanentNegative);
  EXPECT_TRUE(response.CodeEquals(550));
  EXPECT_TRUE(response.StatusEquals(StatusType::PermanentNegative));
}

// Test Case 5: Valid SMTP Response with enhanced code
TEST(SMTPResponseTest, EnhancedCodeResponse) {
  auto result = SMTPResponse::Create("250 2.1.5 Recipient OK");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_code(), 250);
  EXPECT_EQ(response.get_enhanced_code(), "2.1.5");
  EXPECT_EQ(response.get_text(), "Recipient OK");
  EXPECT_EQ(response.get_status(), StatusType::PositiveCompletion);
  EXPECT_TRUE(response.CodeEquals(250));
  EXPECT_TRUE(response.StatusEquals(StatusType::PositiveCompletion));
}

// Test Case 6: Invalid SMTP Response with non-numeric code
TEST(SMTPResponseTest, InvalidResponseFormat) {
  auto result = SMTPResponse::Create("ABC OK");
  EXPECT_FALSE(result);
  EXPECT_EQ(result.error(), "Invalid response format");
}

// Test Case 7: Invalid SMTP Response with incomplete code
TEST(SMTPResponseTest, InvalidResponseIncompleteCode) {
  auto result = SMTPResponse::Create("25 OK");
  EXPECT_FALSE(result);
  EXPECT_EQ(result.error(), "Invalid response format");
}

// Test Case 8: Valid SMTP Response with unknown status code
TEST(SMTPResponseTest, UnknownStatusCode) {
  auto result = SMTPResponse::Create("999 Custom response");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_code(), 999);
  EXPECT_EQ(response.get_enhanced_code(), "");
  EXPECT_EQ(response.get_text(), "Custom response");
  EXPECT_EQ(response.get_status(), StatusType::Undefined);
  EXPECT_TRUE(response.CodeEquals(999));
  EXPECT_TRUE(response.StatusEquals(StatusType::Undefined));
}

// Test Case 9: Valid SMTP Response with multiple lines
TEST(SMTPResponseTest, MultipleLinesResponse) {
  auto result =
      SMTPResponse::Create("250 1.4.1 First line\r\nSecond line\r\nThird line");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_code(), 250);
  EXPECT_EQ(response.get_enhanced_code(), "1.4.1");
  EXPECT_EQ(response.get_text(), "First line\r\nSecond line\r\nThird line");
  EXPECT_EQ(response.get_status(), StatusType::PositiveCompletion);
  EXPECT_TRUE(response.CodeEquals(250));
  EXPECT_TRUE(response.StatusEquals(StatusType::PositiveCompletion));
}

// Test Case 10: Valid SMTP Response with formatted response
TEST(SMTPResponseTest, FormattedResponse) {
  auto result = SMTPResponse::Create("250 OK\r\n");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_formated_response(), "S: 250 OK\r\n");
}

// Test Case 11: Valid SMTP Response with formatted response with multiple lines
TEST(SMTPResponseTest, FormattedResponseMultipleLines) {
  auto result = SMTPResponse::Create("250 OK\r\nSecond line\r\nThird line\r\n");
  ASSERT_TRUE(result);

  SMTPResponse response = std::move(result.value());
  EXPECT_EQ(response.get_formated_response(),
            "S: 250 OK\r\nS: Second line\r\nS: Third line\r\n");
}