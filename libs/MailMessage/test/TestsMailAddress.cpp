#include <gtest/gtest.h>

#include "MailAddress.hpp"

using namespace ISXMM;

TEST(MailAddressTest, ValidEmail) {
  MailAddress address("example@example.com", "John Doe");
  EXPECT_TRUE(address.is_valid());
  EXPECT_EQ(address.get_address(), "example@example.com");
  EXPECT_EQ(address.get_name(), "John Doe");
}

TEST(MailAddressTest, InvalidEmail) {
  MailAddress address("invalid-email", "Jane Doe");
  EXPECT_FALSE(address.is_valid());
  EXPECT_EQ(address.get_address(), "invalid-email");
  EXPECT_EQ(address.get_name(), "Jane Doe");
}

TEST(MailAddressTest, EmptyEmail) {
  MailAddress address("", "No Name");
  EXPECT_TRUE(address.is_valid());
  EXPECT_EQ(address.get_address(), "");
  EXPECT_EQ(address.get_name(), "No Name");
}

TEST(MailAddressTest, ValidEmailWithoutName) {
  MailAddress address("example@example.com", "");
  EXPECT_TRUE(address.is_valid());
  EXPECT_EQ(address.get_address(), "example@example.com");
  EXPECT_EQ(address.get_name(), "");
}

TEST(MailAddressTest, InvalidEmailWithSpecialCharacters) {
  MailAddress address("example@.com", "Invalid Format");
  EXPECT_FALSE(address.is_valid());
  EXPECT_EQ(address.get_address(), "example@.com");
  EXPECT_EQ(address.get_name(), "Invalid Format");
}
