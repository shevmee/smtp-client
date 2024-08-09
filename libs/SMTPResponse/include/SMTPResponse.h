#pragma once 

#include <string>
#include <regex>

#include "StatusType.h"

namespace ISXR
{

    class SMTPResponse {
    public:
        explicit SMTPResponse(const std::string& response);
    
        u_int16_t get_code() const;
        std::string get_enhanced_code() const;
        std::string get_text() const;
    
        StatusType get_status() const;
        bool CodeEquals(u_int16_t code) const;
        bool StatusEquals(StatusType status) const;
        std::string get_formated_response() const;
    
    private:
        u_int16_t m_code;
        std::string m_enhanced_code;
        std::string m_text;
        std::string m_formated_response;
        StatusType m_status;
    
        void ParseResponse(const std::string& response);
        void FormatResponse(const std::string& response);
        bool IsValidResponse(const std::string& response, std::smatch& matches) const;
    };

} // namespace ISXR