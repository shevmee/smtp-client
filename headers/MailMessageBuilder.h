#include <vector>
#include <string>

class MailAddress {};
class MailMessage {};

namespace ISXSC
{
    class MailMessageBuilder
    {
    public:
        MailMessageBuilder& SetFrom(const MailAddress& from);
        MailMessageBuilder& SetTo(const MailAddress& to);
        MailMessageBuilder& SetCc(const MailAddress& cc);
        MailMessageBuilder& SetBcc(const MailAddress& bcc);
        MailMessageBuilder& SetSubject(const std::string& subject);
        MailMessageBuilder& SetBody(const std::string& body);
        // MailMessageBuilder& SetAttachment(const Attachment& attachment);
        MailMessage Build();

    private:
        MailAddress m_from;
        std::vector<MailAddress> m_to;
        std::vector<MailAddress> m_cc;
        std::vector<MailAddress> m_bcc;
        std::string m_subject;
        std::string m_body;
        // std::vector<Attachment> attachments;
    };
}