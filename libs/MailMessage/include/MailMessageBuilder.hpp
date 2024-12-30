#pragma once

#include <string>
#include <vector>

#include "MailAddress.hpp"
#include "MailAttachment.hpp"
#include "MailMessage.hpp"

namespace ISXMM {
class MailMessageBuilder {
 public:
  MailMessageBuilder &set_from(const std::string &email,
                               const std::string &name = "");
  MailMessageBuilder &add_to(const std::string &email,
                             const std::string &name = "");
  MailMessageBuilder &add_cc(const std::string &email,
                             const std::string &name = "");
  MailMessageBuilder &add_bcc(const std::string &email,
                              const std::string &name = "");
  MailMessageBuilder &set_subject(const std::string &subject);
  MailMessageBuilder &set_body(const std::string &body);
  MailMessageBuilder &add_attachment(const std::string &path);
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