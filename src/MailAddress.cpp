#include "MailAddress.h"
#include <format>

namespace ISXSC
{
    MailAddress::MailAddress(const std::string& address): m_address(address), m_name(""){}
    
    MailAddress::MailAddress(const std::string& address, const std::string& name): m_address(address), m_name(name){}
    
    MailAddress::operator std::string() const
    {
        if (!m_name.empty())
        {
            return std::format("{} <{}>", m_name, m_address);
        }
        else 
        {
            return m_address;
        }
    }
}