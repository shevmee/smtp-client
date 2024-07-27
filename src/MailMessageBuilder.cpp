#include "MailMessageBuilder.h"

#include "MailAddress.h"
#include "MailMessage.h"

namespace ISXSC
{
    MailMessageBuilder& MailMessageBuilder::SetFrom(const std::string& email, const std::string& name)
    {
        m_from = { email, name };
        return *this;
    }

    MailMessageBuilder& MailMessageBuilder::AddTo(const std::string& email, const std::string& name)
    {
        m_to.push_back({ email, name });
        return *this;
    }

    MailMessageBuilder& MailMessageBuilder::AddCc(const std::string& email, const std::string& name)
    {
        m_cc.push_back({ email, name });
        return *this;
    }

    MailMessageBuilder& MailMessageBuilder::AddBcc(const std::string& email, const std::string& name)
    {
        m_bcc.push_back({ email, name });
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
        if (m_from.get_address().empty())
        {
            // throw error
        }

        MailMessage message { m_from, m_to, m_cc, m_bcc, m_subject, m_body };
        return message;
    }
}