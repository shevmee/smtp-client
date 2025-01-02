#include "MailAddress.hpp"

namespace ISXMM {
MailAddress::MailAddress(std::string_view address, std::string_view name)
    : m_address(address), m_name(name) {}

std::string_view MailAddress::get_address() const { return m_address; }

std::string_view MailAddress::get_name() const { return m_name; }
}  // namespace ISXMM