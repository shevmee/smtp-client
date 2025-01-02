#include "MailMessageFormatter.hpp"

#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "MailAddress.hpp"
#include "MailAttachment.hpp"
#include "MailMessage.hpp"

namespace ISXMM {
namespace {

std::string FormatAddressList(const std::vector<MailAddress> &addresses,
                              const std::string &prefix) {
  if (addresses.empty()) {
    return "";
  }

  std::string result = prefix;
  for (const auto &address : addresses) {
    if (!address.is_valid()) {
      std::cerr << "Warning: Skipping invalid email address: "
                << address.get_address() << std::endl;
      continue;
    }

    if (address.get_name().empty()) {
      result += std::format("{}, ", address.get_address());
    } else {
      result +=
          std::format("{} <{}>, ", address.get_name(), address.get_address());
    }
  }

  if (!result.empty()) {
    result.erase(result.length() - 2, 2);
  }
  result += "\r\n";
  return result;
}

}  // namespace

std::string MailMessageFormatter::MailFrom(const MailAddress &from) {
  if (!from.is_valid()) {
    std::cerr << "Warning: Invalid 'From' email address: " << from.get_address()
              << std::endl;
    return "";
  }

  return from.get_name().empty()
             ? std::format("From: {}\r\n", from.get_address())
             : std::format("From: {} <{}>\r\n", from.get_name(),
                           from.get_address());
}

std::string MailMessageFormatter::MailTo(const std::vector<MailAddress> &to) {
  return FormatAddressList(to, "To: ");
}

std::string MailMessageFormatter::MailCc(const std::vector<MailAddress> &cc) {
  return FormatAddressList(cc, "Cc: ");
}

std::string MailMessageFormatter::MailHeaders(const MailMessage &message) {
  std::string headers = MailFrom(message.from) + MailTo(message.to) +
                        MailCc(message.cc) +
                        std::format("Subject: {}\r\n", message.subject);

  if (!message.attachments.empty()) {
    headers += std::format(
        "MIME-Version: 1.0\r\nContent-Type: multipart/mixed; "
        "boundary=\"{}\"\r\n",
        boundary);
  }

  headers += "\r\n";
  return headers;
}

std::string MailMessageFormatter::MailBody(const MailMessage &message) {
  std::string body;
  if (!message.attachments.empty()) {
    body += std::format(
        "--{}\r\nContent-Type: text/plain; "
        "charset=\"UTF-8\"\r\nContent-Transfer-Encoding: 7bit\r\n\r\n",
        boundary);
  }
  body += message.body + "\r\n";
  return body;
}

std::string MailMessageFormatter::MailAttachmentHeaders(
    const MailAttachment &attachment, const std::string &filetype) {
  return std::format(
      "Content-Type: {}; name=\"{}\"\r\nContent-Transfer-Encoding: "
      "base64\r\nContent-Disposition: attachment; filename=\"{}\"\r\n\r\n",
      filetype, attachment.get_name(), attachment.get_name());
}

}  // namespace ISXMM