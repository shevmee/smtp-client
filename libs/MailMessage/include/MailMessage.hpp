#pragma once

#include <vector>
#include <string>
#include "MailAddress.hpp"
#include "MailAttachment.hpp"

namespace ISXMM
{
struct MailMessage
{
    MailAddress from;
    std::vector<MailAddress> to;
    std::vector<MailAddress> cc;
    std::vector<MailAddress> bcc;
    std::string subject;
    std::string body;
    std::vector<MailAttachment> attachments;
};
} // namespace ISXMM