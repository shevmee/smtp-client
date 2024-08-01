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

std::string Base64Encode(const std::string& decoded) {
    auto encoded_size = boost::beast::detail::base64::encoded_size(decoded.size());
    std::string encoded_output(encoded_size, '\0');
    boost::beast::detail::base64::encode(encoded_output.data(), decoded.data(), decoded.size());

    for (size_t i = 500; i < encoded_output.size(); i += 502) {
        encoded_output.insert(i, "\r\n");
    }

    return encoded_output;
}

std::string ReadFile(const std::string& filename)
{
    std::ifstream t(filename);
    std::string str;

    t.seekg(0, std::ios::end);   
    uint32_t size = t.tellg();
    t.seekg(0, std::ios::beg);

    if (size > ISXSC::MailAttachment::S_MAX_SIZE) {
        throw std::runtime_error("File is too big");
    }

    str.reserve(size);

    str.assign((std::istreambuf_iterator<char >(t)),
    std::istreambuf_iterator<char>());
    return str;
}

namespace ISXSC {

    std::string MailMessageFormatter::MailFormat(const MailMessage& message) {
        std::ostringstream formatted_message;
        formatted_message << MailHeaders(message)
                          << MailBody(message)
                          << MailAttachments(message.attachments);

        return formatted_message.str();
    }

    std::string MailMessageFormatter::MailFrom(const MailAddress& from) {
        return "From: " + from.get_name() + " <" + from.get_address() + ">\r\n";
    }

    std::string MailMessageFormatter::MailTo(const std::vector<MailAddress>& to) {
        std::ostringstream to_stream;
        to_stream << "To: ";
        for (const auto& address : to) {
            to_stream << address.get_name() << " <" << address.get_address() << ">, ";
        }
        std::string to_string = to_stream.str();
        to_string.erase(to_string.length() - 2, 2);
        to_string += "\r\n";

        return to_string;
    }

    std::string MailMessageFormatter::MailCc(const std::vector<MailAddress>& cc) {
        if (cc.empty()) {
            return "";
        }

        std::ostringstream cc_stream;
        cc_stream << "CC: ";
        for (const auto& address : cc) {
            cc_stream << address.get_address() << " <" << address.get_name() << ">, ";
        }
        std::string cc_string = cc_stream.str();
        cc_string.erase(cc_string.length() - 2, 2);
        cc_string += "\r\n";

        return cc_string;
    }

    std::string MailMessageFormatter::MailHeaders(const MailMessage& message) {
        std::ostringstream headers;
        headers << MailFrom(message.from)
                << MailTo(message.to)
                << MailCc(message.cc)
                << "Subject: " + message.subject + "\r\n";

        if (!message.attachments.empty()) {
            headers << "MIME-Version: 1.0\r\n"
                    << "Content-Type: multipart/mixed; boundary=\"boundary\"\r\n";
        }

        headers << "\r\n";
        return headers.str();
    }

    std::string MailMessageFormatter::MailBody(const MailMessage& message) {
        std::ostringstream body;

        if (!message.attachments.empty()) {
            body << "--boundary\r\n"
                 << "Content-Type: text/plain; charset=\"UTF-8\"\r\n"
                 << "Content-Transfer-Encoding: 7bit\r\n\r\n";
        }

        body << message.body + "\r\n";
        return body.str();
    }

    std::string MailMessageFormatter::MailAttachments(const std::vector<MailAttachment>& attachments) {
        if (attachments.empty()) {
            return "";
        }

        std::ostringstream formatted_attachments;
        for (const auto& attachment : attachments) {
            formatted_attachments << "--boundary\r\n"
                                  << "Content-Type: application/octet-stream; name=\"" + attachment.get_name() + "\"\r\n"
                                  << "Content-Transfer-Encoding: base64\r\n"
                                  << "Content-Disposition: attachment; filename=\"" + attachment.get_name() + "\"\r\n\r\n"
                                  << Base64Encode(ReadFile(attachment.get_path())) + "\r\n";
        }

        formatted_attachments << "--boundary--\r\n";
        return formatted_attachments.str();
    }
}