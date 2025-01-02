#include <expected>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>

#include "Base64.hpp"
#include "MailMessageFormatter.hpp"

namespace ISXMS {

class MessageSender {
 public:
  MessageSender(const ISXMM::MailMessage &message,
                std::function<bool(const std::string &)> send);

  std::expected<void, std::string> SendMessage();

 private:
  static uint16_t inline S_FILE_CHUNK_SIZE = 72;
  ISXMM::MailMessage m_message;
  std::function<bool(const std::string &)> m_send;

  std::expected<void, std::string> SendMailHeaders();
  std::expected<void, std::string> SendMailBody();
  std::expected<void, std::string> SendAttachments();

  std::expected<void, std::string> SendFile(const std::filesystem::path &path);
};

};  // namespace ISXMS