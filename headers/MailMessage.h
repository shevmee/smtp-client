#include <vector>
#include <string>

class MailAddress {};

namespace ISXSC
{
    struct MailMessage
    {
        MailAddress from;
        std::vector<MailAddress> to;
        std::vector<MailAddress> cc;
        std::vector<MailAddress> bcc;
        std::string subject;
        std::string body;
        // std::vector<Attachment> attachments;
    };
}