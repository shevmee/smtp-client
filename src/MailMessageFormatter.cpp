#include "MailMessageFormatter.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <boost/beast/core/detail/base64.hpp>
#include "MailMessage.h"
#include "MailAddress.h"
#include "MailAttachment.h"


std::string Base64Encode(const std::string& decoded)
    {
        auto encoded_size = boost::beast::detail::base64::encoded_size(decoded.size());
        std::string encoded_output(encoded_size, '\0');
        boost::beast::detail::base64::encode(encoded_output.data(), decoded.data(), decoded.size());
        // add endlines every 500 characters to prevent long lines
        for (size_t i = 500; i < encoded_output.size(); i += 500)
        {
            encoded_output.insert(i, "\r\n");
            i+=2;
        }
        return encoded_output;
    };

std::string Readfile(const std::string& filename)
{
    std::ifstream t(filename);
    std::string str;

    t.seekg(0, std::ios::end);   
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char >(t)),
    std::istreambuf_iterator<char>());
    return str;
}

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
            formatted_attachments += Base64Encode(Readfile(attachment.get_path())) + "\r\n";
        }

        if (!attachments.empty())
        {
            formatted_attachments += "--" + boundary + "--\r\n";
        }

        return formatted_attachments;
    }
}