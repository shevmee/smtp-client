#pragma once

#include <string>

namespace ISXMM {
class MailAddress {
 public:
  MailAddress(std::string_view address = "", std::string_view name = "");

  std::string_view get_address() const;
  std::string_view get_name() const;
  bool is_valid() const;

 private:
  std::string m_address;
  std::string m_name;
  bool m_is_valid;

  bool is_valid_email(std::string_view email) const;
};
}  // namespace ISXMM