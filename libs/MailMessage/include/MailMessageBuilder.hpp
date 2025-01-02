#pragma once

#include <string>
#include <vector>

#include "MailAddress.hpp"
#include "MailAttachment.hpp"
#include "MailMessage.hpp"

namespace ISXMM {
class MailMessageBuilder {
 public:
  MailMessageBuilder &set_from(std::string_view email,
                               std::string_view name = "");
  MailMessageBuilder &add_to(std::string_view email,
                             std::string_view name = "");
  MailMessageBuilder &add_cc(std::string_view email,
                             std::string_view name = "");
  MailMessageBuilder &add_bcc(std::string_view email,
                              std::string_view name = "");
  MailMessageBuilder &set_subject(std::string_view subject);
  MailMessageBuilder &set_body(std::string_view body);
  MailMessageBuilder &add_attachment(std::string_view path);
  MailMessage Build();

 private:
  MailAddress m_from;
  std::vector<MailAddress> m_to = {};
  std::vector<MailAddress> m_cc = {};
  std::vector<MailAddress> m_bcc = {};
  std::string m_subject;
  std::string m_body;
  std::vector<MailAttachment> m_attachments;
};
}  // namespace ISXMM