#include "MessageSender.hpp"

#include <expected>  // C++23 feature
#include <ranges>    // C++23 feature

namespace ISXMS {

MessageSender::MessageSender(const ISXMM::MailMessage &message,
                             std::function<bool(const std::string &)> send)
    : m_message(message), m_send(send) {}

std::expected<void, std::string> MessageSender::SendMessage() {
  if (!SendMailHeaders()) return std::unexpected{"Failed to send mail headers"};
  if (!SendMailBody()) return std::unexpected{"Failed to send mail body"};
  if (!SendAttachments()) return std::unexpected{"Failed to send attachments"};
  return {};
}

std::expected<void, std::string> MessageSender::SendFile(
    const std::filesystem::path &file_path) {
  std::ifstream file(file_path, std::ios::binary);
  std::vector<char> buffer(S_FILE_CHUNK_SIZE);

  if (!file) return std::unexpected{"Failed to open file"};

  if (std::filesystem::file_size(file_path) > ISXMM::MailAttachment::S_MAX_SIZE)
    return std::unexpected{"File size is too large or inaccessible"};

  while (!file.eof()) {
    file.read(buffer.data(), buffer.size());
    std::string encoded_buffer =
        ISXBase64::Base64Encode(
            std::string(buffer.begin(), buffer.begin() + file.gcount())) +
        "\r\n";
    if (!m_send(encoded_buffer))
      return std::unexpected{"Failed to send encoded buffer"};
  };
  return {};
}

std::expected<void, std::string> MessageSender::SendAttachments() {
  if (m_message.attachments.empty()) return {};

  for (const auto &attachment : m_message.attachments | std::views::all) {
    if (!m_send("--" + ISXMM::MailMessageFormatter::boundary + "\r\n"))
      return std::unexpected{"Failed to send attachment boundary"};
    if (!m_send(ISXMM::MailMessageFormatter::MailAttachmentHeaders(attachment)))
      return std::unexpected{"Failed to send attachment headers"};
    if (!SendFile(attachment.get_path()))
      return std::unexpected{"Failed to send file"};
  }

  if (!m_send("--" + ISXMM::MailMessageFormatter::boundary + "--\r\n"))
    return std::unexpected{"Failed to send final boundary"};

  return {};
}

std::expected<void, std::string> MessageSender::SendMailHeaders() {
  if (!m_send(ISXMM::MailMessageFormatter::MailHeaders(m_message)))
    return std::unexpected{"Failed to send mail headers"};
  return {};
}

std::expected<void, std::string> MessageSender::SendMailBody() {
  if (!m_send(ISXMM::MailMessageFormatter::MailBody(m_message)))
    return std::unexpected{"Failed to send mail body"};
  return {};
}

};  // namespace ISXMS