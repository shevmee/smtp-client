#include "MailAttachment.hpp"

#include <fstream>
#include <streambuf>
#include <string>

namespace ISXMM {
MailAttachment::MailAttachment(const std::filesystem::path &path)
    : m_path(path) {}

std::filesystem::path MailAttachment::get_path() const { return m_path; }

std::string MailAttachment::get_name() const {
  return m_path.filename().string();
}
}  // namespace ISXMM