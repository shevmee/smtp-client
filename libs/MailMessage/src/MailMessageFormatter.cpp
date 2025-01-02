#include "MailMessageFormatter.hpp"

#include <format>
#include <string>
#include <vector>

#include "MailAddress.hpp"
#include "MailAttachment.hpp"
#include "MailMessage.hpp"

namespace ISXMM {

std::string MailMessageFormatter::MailFrom(const MailAddress &from) {
  if (from.get_name().empty()) {
    return std::format("From: {}\r\n", from.get_address());
  }
  return std::format("From: {} <{}>\r\n", from.get_name(), from.get_address());
}

std::string MailMessageFormatter::MailTo(const std::vector<MailAddress> &to) {
  std::string to_list = "To: ";
  for (const auto &address : to) {
    if (address.get_name().empty()) {
      to_list += std::format("{}, ", address.get_address());
    } else {
      to_list +=
          std::format("{} <{}>, ", address.get_name(), address.get_address());
    }
  }
  if (!to_list.empty()) {
    to_list.erase(to_list.length() - 2, 2);  // Remove trailing comma and space
  }
  to_list += "\r\n";
  return to_list;
}

std::string MailMessageFormatter::MailCc(const std::vector<MailAddress> &cc) {
  if (cc.empty()) {
    return "";
  }
  std::string cc_list = "Cc: ";
  for (const auto &address : cc) {
    if (address.get_name().empty()) {
      cc_list += std::format("{}, ", address.get_address());
    } else {
      cc_list +=
          std::format("{} <{}>, ", address.get_name(), address.get_address());
    }
  }
  if (!cc_list.empty()) {
    cc_list.erase(cc_list.length() - 2, 2);  // Remove trailing comma and space
  }
  cc_list += "\r\n";
  return cc_list;
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