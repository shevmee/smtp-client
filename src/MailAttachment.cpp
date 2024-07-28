#include "MailAttachment.h"

#include <fstream>
#include <string>
#include <streambuf>

namespace ISXSC
{
    MailAttachment::MailAttachment(const std::filesystem::path& path): m_path(path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file");
        }

        // determine file size in advance
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);

        if (length > S_MAX_SIZE)
        {
            file.close();
            throw std::runtime_error("File is too large");
        }

        m_data.reserve(length);
        m_data.assign(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));

        file.close();
    }

    std::filesystem::path MailAttachment::get_path() const
    {
        return m_path;
    }

    std::string MailAttachment::get_name() const
    {
        return m_path.filename().string();
    }

    std::string MailAttachment::get_data() const
    {
        return m_data;
    }
}