#pragma once

#include <vector>
#include <string>
#include <initializer_list>

#include "MailAddress.h"
#include "MailMessage.h"

namespace ISXSC
{
    class MailMessageBuilder
    {
    public:
        MailMessageBuilder& SetFrom(const MailAddress& from);
        MailMessageBuilder& AddTo(const MailAddress& to);
        MailMessageBuilder& AddCc(const MailAddress& cc);
        MailMessageBuilder& AddBcc(const MailAddress& bcc);
        MailMessageBuilder& SetSubject(const std::string& subject);
        MailMessageBuilder& SetBody(const std::string& body);
        // MailMessageBuilder& SetAttachment(const Attachment& attachment);
        MailMessage Build();

    private:
        MailAddress m_from;
        std::vector<MailAddress> m_to = {};
        std::vector<MailAddress> m_cc = {};
        std::vector<MailAddress> m_bcc = {};
        std::string m_subject;
        std::string m_body;
        // std::vector<Attachment> attachments;
    };
}