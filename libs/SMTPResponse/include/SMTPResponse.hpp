#pragma once

#include <expected>
#include <regex>
#include <string>

#include "StatusType.hpp"

namespace ISXResponse {
class SMTPResponse {
 public:
  SMTPResponse() = default;
  static std::expected<SMTPResponse, std::string> Create(
      const std::string &response);

  u_int16_t get_code() const;
  std::string get_enhanced_code() const;
  std::string get_text() const;

  StatusType get_status() const;
  bool CodeEquals(u_int16_t code) const;
  bool StatusEquals(StatusType status) const;
  std::string get_formated_response() const;
  std::string get_raw_response() const;

  static std::expected<void, std::string> CheckStatus(
      const SMTPResponse &response, StatusType status);

 private:
  std::expected<void, std::string> ParseResponse(const std::string &response);
  void FormatResponse(const std::string &response);
  bool IsValidResponse(const std::string &response, std::smatch &matches) const;
  void DetermineStatus();
  auto SplitAtEndline(const std::string &response) const
      -> std::pair<std::string, std::string>;

  u_int16_t m_code{0};
  std::string m_enhanced_code;
  std::string m_text;
  std::string m_formated_response;
  std::string m_raw_response;
  StatusType m_status{StatusType::Undefined};
};
}  // namespace ISXResponse