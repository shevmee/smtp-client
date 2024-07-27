#include "MailMessageBuilder.h"

namespace ISXSC
{
    MailMessageBuilder& MailMessageBuilder::SetFrom(const MailAddress& from)
    {
        m_from = from;
        return *this;
    }

    MailMessageBuilder& MailMessageBuilder::AddTo(const MailAddress& to)
    {
        m_to.push_back(to);
        return *this;
    }

    MailMessageBuilder& MailMessageBuilder::AddCc(const MailAddress& cc)
    {
        m_cc.push_back(cc);
        return *this;
    }

    MailMessageBuilder& MailMessageBuilder::AddBcc(const MailAddress& bcc)
    {
        m_bcc.push_back(bcc);
        return *this;
    }

    MailMessageBuilder& MailMessageBuilder::SetSubject(const std::string& subject)
    {
        m_subject = subject;
        return *this;
    }

    MailMessageBuilder& MailMessageBuilder::SetBody(const std::string& body)
    {
        m_body = body;
        return *this;
    }

    MailMessage MailMessageBuilder::Build()
    {
        // Implement once branch is merged with MailMessage.h and MailAddress.h
    }
}