#include "MailAddress.hpp"

#include <format>
#include <iostream>
#include <regex>

namespace ISXMM {
MailAddress::MailAddress(std::string_view address, std::string_view name)
    : m_address(address), m_name(name), m_is_valid(true) {
  if (!m_address.empty() && !is_valid_email(m_address)) {
    m_is_valid = false;
  }
}

std::string_view MailAddress::get_address() const { return m_address; }

std::string_view MailAddress::get_name() const { return m_name; }

bool MailAddress::is_valid() const { return m_is_valid; }
bool MailAddress::is_valid_email(std::string_view email) const {
  const std::regex pattern(R"((^[^\s@]+@[^\s@]+\.[^\s@]+$))",
                           std::regex::icase);
  return std::regex_match(email.begin(), email.end(), pattern);
}
}  // namespace ISXMM