#include "MailMessageBuilder.hpp"

#include "MailAddress.hpp"
#include "MailAttachment.hpp"
#include "MailMessage.hpp"

namespace ISXMM {
MailMessageBuilder &MailMessageBuilder::set_from(std::string_view email,
                                                 std::string_view name) {
  m_from = {email, name};
  return *this;
}

MailMessageBuilder &MailMessageBuilder::add_to(std::string_view email,
                                               std::string_view name) {
  m_to.emplace_back(email, name);
  return *this;
}

MailMessageBuilder &MailMessageBuilder::add_cc(std::string_view email,
                                               std::string_view name) {
  m_cc.emplace_back(email, name);
  return *this;
}

MailMessageBuilder &MailMessageBuilder::add_bcc(std::string_view email,
                                                std::string_view name) {
  m_bcc.emplace_back(email, name);
  return *this;
}

MailMessageBuilder &MailMessageBuilder::set_subject(std::string_view subject) {
  m_subject = subject;
  return *this;
}

MailMessageBuilder &MailMessageBuilder::set_body(std::string_view body) {
  m_body = body;
  return *this;
}

MailMessageBuilder &MailMessageBuilder::add_attachment(std::string_view path) {
  m_attachments.emplace_back(path);

  return *this;
}

MailMessage MailMessageBuilder::Build() {
  if (m_from.get_address().empty() || m_to.empty()) {
    throw std::runtime_error("Not all required fields are filled");
  }
  MailMessage message{m_from,    m_to,   m_cc,         m_bcc,
                      m_subject, m_body, m_attachments};
  return message;
}
}  // namespace ISXMM