#include "SMTPResponse.h"

namespace ISXR
{
    SMTPResponse::SMTPResponse(const std::string& response) 
    {
        ParseResponse(response);

        switch (m_code / 100) 
        {
        case 2:
            m_status = StatusType::PositiveCompletion;
            break;
        case 3:
            m_status = StatusType::PositiveIntermediate;
            break;
        case 4:
            m_status = StatusType::TransientNegative;
            break;
        case 5:
            m_status = StatusType::PermanentNegative;
            break;
        default:
            m_status = StatusType::Undefined;
            break;
        }
    }

    u_int16_t SMTPResponse::get_code() const 
    {
        return m_code;
    }

    std::string SMTPResponse::get_enhanced_code() const 
    {
        return m_enhanced_code;
    }

    std::string SMTPResponse::get_text() const 
    {
        return m_text;
    }

    StatusType SMTPResponse::get_status() const 
    {
        return m_status;
    }

    void SMTPResponse::ParseResponse(const std::string& response) 
    {
        std::regex responsePattern(R"(^(\d{3})(?:[ -](\d\.\d\.\d))?[ -](.*)$)");
        std::smatch matches;

        if (std::regex_match(response, matches, responsePattern)) 
        {
            if (matches.size() > 1) 
            {
                m_code = std::stoi(matches[1].str());
            }
            if (matches.size() > 2 && matches[2].matched) 
            {
                m_enhanced_code = matches[2].str();
            }
            if (matches.size() > 3 && matches[3].matched) 
            {
                m_text = matches[3].str();
            }
        } else 
        {
            throw std::invalid_argument("Invalid response format");
        }
    }

    bool SMTPResponse::CodeEquals(u_int16_t code) const 
    {
        return m_code == code;
    }

    bool SMTPResponse::StatusEquals(StatusType status) const 
    {
        return m_status == status;
    }

} // namespace SMTPResonse