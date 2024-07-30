#include <gtest/gtest.h>
#include "MailMessageBuilder.h"
#include "MailAddress.h"
#include "MailMessage.h"
#include "MailAttachment.h"

TEST(MailMessageBuilderTest, SetFrom) {
    ISXSC::MailMessageBuilder builder;
    builder.SetFrom("sender@example.com", "Sender Name")
        .AddTo("placeholder");
    
    ISXSC::MailMessage message = builder.Build();
    EXPECT_EQ(message.from.get_address(), "sender@example.com");
    EXPECT_EQ(message.from.get_name(), "Sender Name");
}

TEST(MailMessageBuilderTest, AddTo) {
    ISXSC::MailMessageBuilder builder;
    builder.SetFrom("sender@example.com", "Sender Name")
           .AddTo("recipient@example.com", "Recipient Name");
    
    ISXSC::MailMessage message = builder.Build();
    EXPECT_EQ(message.to.size(), 1);
    EXPECT_EQ(message.to.at(0).get_address(), "recipient@example.com");
    EXPECT_EQ(message.to.at(0).get_name(), "Recipient Name");
}

TEST(MailMessageBuilderTest, AddCc) {
    ISXSC::MailMessageBuilder builder;
    builder.SetFrom("sender@example.com", "Sender Name")
           .AddCc("cc@example.com", "Cc Name")
           .AddTo("placeholder");
    
    ISXSC::MailMessage message = builder.Build();
    EXPECT_EQ(message.cc.size(), 1);
    EXPECT_EQ(message.cc.at(0).get_address(), "cc@example.com");
    EXPECT_EQ(message.cc.at(0).get_name(), "Cc Name");
}

TEST(MailMessageBuilderTest, AddBcc) {
    ISXSC::MailMessageBuilder builder;
    builder.SetFrom("sender@example.com", "Sender Name")
           .AddBcc("bcc@example.com", "Bcc Name")
           .AddTo("placeholder");
    
    ISXSC::MailMessage message = builder.Build();
    EXPECT_EQ(message.bcc.size(), 1);
    EXPECT_EQ(message.bcc.at(0).get_address(), "bcc@example.com");
    EXPECT_EQ(message.bcc.at(0).get_name(), "Bcc Name");
}

TEST(MailMessageBuilderTest, SetSubject) {
    ISXSC::MailMessageBuilder builder;
    builder.SetFrom("sender@example.com", "Sender Name")
           .SetSubject("Test Subject")
           .AddTo("placeholder");
    
    ISXSC::MailMessage message = builder.Build();
    EXPECT_EQ(message.subject, "Test Subject");
}

TEST(MailMessageBuilderTest, SetBody) {
    ISXSC::MailMessageBuilder builder;
    builder.SetFrom("sender@example.com", "Sender Name")
           .SetBody("Test Body")
           .AddTo("placeholder");
    
    ISXSC::MailMessage message = builder.Build();
    EXPECT_EQ(message.body, "Test Body");
}

TEST(MailMessageBuilderTest, AddAttachment) {
    ISXSC::MailMessageBuilder builder;
    builder.SetFrom("sender@example.com", "Sender Name")
           .AddAttachment("/path/to/attachment.txt")
           .AddTo("placeholder");
    
    ISXSC::MailMessage message = builder.Build();
    EXPECT_EQ(message.attachments.size(), 1);
    EXPECT_EQ(message.attachments.at(0).get_path(), "/path/to/attachment.txt");
}

TEST(MailMessageBuilderTest, BuildThrowsIfFromNotSet) {
    ISXSC::MailMessageBuilder builder;
    builder.AddTo("recipient@example.com", "Recipient Name");
    
    EXPECT_THROW(builder.Build(), std::runtime_error);
}

TEST(MailMessageBuilderTest, BuildThrowsIfToNotSet) {
    ISXSC::MailMessageBuilder builder;
    builder.SetFrom("sender@example.com", "Sender Name");
    
    EXPECT_THROW(builder.Build(), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}