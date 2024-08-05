#pragma once
#include "MailMessageBuilder.h"

namespace ISXMM
{
    class MailMessageFormatter
    {
    public:
        inline static std::string boundary = "NEXT_PART_My4xNDE1OTI2NTM1";
        static std::string MailFrom(const MailAddress& from);
        static std::string MailTo(const std::vector<MailAddress>& to);
        static std::string MailCc(const std::vector<MailAddress>& cc);
        static std::string MailHeaders(const MailMessage& message);
        static std::string MailBody(const MailMessage& message);
        static std::string MailAttachmentHeaders(const MailAttachment& attachment, const std::string& filetype = "application/octet-stream");
    };
}