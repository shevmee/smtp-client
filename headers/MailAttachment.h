#pragma once

#include <string>
#include <filesystem>

namespace ISXSC
{
    class MailAttachment
    {
    public:
        MailAttachment(const std::filesystem::path& path = "");
    
        operator std::string() const;
    
        std::filesystem::path get_path() const;
        std::string get_name() const;
        std::string get_data() const;
    private:
        std::filesystem::path m_path;
        std::string m_data;
        static inline constexpr uint32_t S_MAX_SIZE = 5 * 1024 * 1024;
    };
}