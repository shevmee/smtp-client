#pragma once
#include "MailMessageBuilder.h"

namespace ISXSC
{
    class MailMessageFormatter
    {
    public:
        inline static std::string boundary = "boundary";
        static std::string MailFormat(const MailMessage& message);

    private:
        static std::string MailFrom(const MailAddress& from);
        static std::string MailTo(const std::vector<MailAddress>& to);
        static std::string MailCc(const std::vector<MailAddress>& cc);
        static std::string MailHeaders(const MailMessage& message);
        static std::string MailBody(const MailMessage& message);
        static std::string MailAttachments(const std::vector<MailAttachment>& attachments);
    };
}