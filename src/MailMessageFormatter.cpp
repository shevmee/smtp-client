#include "MailMessageFormatter.h"

#include <vector>
#include <string>
#include <sstream>
#include "MailMessage.h"
#include "MailAddress.h"
#include "MailAttachment.h"


namespace ISXSC
{

    std::string MailMessageFormatter::MailFormat(const MailMessage& message)
    {
        std::string formatted_message = MailHeaders(message);
        formatted_message += MailBody(message);
        formatted_message += MailAttachments(message.attachments);

        return formatted_message;
    }

    std::string MailMessageFormatter::MailFrom(const MailAddress& from)
    {
        return "From: " + from.get_name() + " <" + from.get_address() + ">\r\n";
    }

    std::string MailMessageFormatter::MailTo(const std::vector<MailAddress>& to)
    {
        std::string to_string = "To: ";
        for (const auto& address : to)
        {
            to_string += address.get_name() + " <" + address.get_address() + ">, ";
        }
        to_string.pop_back();
        to_string.pop_back();
        to_string += "\r\n";
        return to_string;
    }

    std::string MailMessageFormatter::MailCc(const std::vector<MailAddress>& cc)
    {
        std::string cc_string = "CC: ";
        for (const auto& address : cc)
        {
            cc_string += address.get_address() + " <" + address.get_name() + ">, ";
        }
        cc_string.pop_back();
        cc_string.pop_back();
        cc_string += "\r\n";
        return cc_string;
    }

    std::string MailMessageFormatter::MailHeaders(const MailMessage& message)
    {
        std::string headers = MailFrom(message.from);
        headers += MailTo(message.to);

        if (!message.cc.empty())
            headers += MailCc(message.cc);

        headers += "Subject: " + message.subject + "\r\n";

        if (!message.attachments.empty())
        {
            headers += "MIME-Version: 1.0\r\n";
            headers += "Content-Type: multipart/mixed; boundary=\"boundary\"\r\n";
        }

        return headers + "\r\n";
    }

    std::string MailMessageFormatter::MailBody(const MailMessage& message)
    {
        std::string body = {};

        if (!message.attachments.empty())
        {
            body += "--" + boundary + "\r\n";
            body += "Content-Type: text/plain; charset=\"UTF-8\"\r\n";
            body += "Content-Transfer-Encoding: 7bit\r\n\r\n";
        }

        body += message.body + "\r\n";

        return body;
    }

    std::string MailMessageFormatter::MailAttachments(const std::vector<MailAttachment>& attachments)
    {
        std::string formatted_attachments = {};

        for (const auto& attachment : attachments)
        {
            
            formatted_attachments += "--" + boundary + "\r\n";
            formatted_attachments += "Content-Type: application/octet-stream; name=\"" + attachment.get_name() + "\"\r\n";
            formatted_attachments += "Content-Transfer-Encoding: base64\r\n";
            formatted_attachments += "Content-Disposition: attachment; filename=\"" + attachment.get_name() + "\"\r\n";
            formatted_attachments += "\r\n";
            formatted_attachments += attachment.get_name() + "(base64 encoded)" + "\r\n";
        }

        if (!attachments.empty())
        {
            formatted_attachments += "--" + boundary + "--\r\n";
        }

        return formatted_attachments;
    }
}