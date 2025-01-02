#include "SMTPResponse.hpp"

#include <format>

namespace ISXResponse {
std::expected<SMTPResponse, std::string> SMTPResponse::Create(
    const std::string &response) {
  SMTPResponse resp;
  if (auto parse_result = resp.ParseResponse(response); !parse_result) {
    return std::unexpected(parse_result.error());
  }
  resp.FormatResponse(response);
  resp.DetermineStatus();
  return resp;
}

u_int16_t SMTPResponse::get_code() const { return m_code; }

std::string SMTPResponse::get_enhanced_code() const { return m_enhanced_code; }

std::string SMTPResponse::get_text() const { return m_text; }

StatusType SMTPResponse::get_status() const { return m_status; }

std::string SMTPResponse::get_formated_response() const {
  return m_formated_response;
}

std::string SMTPResponse::get_raw_response() const { return m_raw_response; }

std::expected<void, std::string> SMTPResponse::CheckStatus(
    const SMTPResponse &response, StatusType status) {
  if (!response.StatusEquals(status)) {
    return std::unexpected(std::format("Unexpected status code in response: {}",
                                       response.get_raw_response()));
  }
  return {};
}

void SMTPResponse::DetermineStatus() {
  switch (m_code / 100) {
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

std::expected<void, std::string> SMTPResponse::ParseResponse(
    const std::string &raw_response) {
  std::smatch matches;
  auto [response, tail] = SplitAtEndline(raw_response);

  if (IsValidResponse(response, matches)) {
    m_raw_response = response;
    if (matches.size() > 1) {
      m_code = std::stoi(matches[1].str());
    }
    if (matches.size() > 2 && matches[2].matched) {
      m_enhanced_code = matches[2].str();
    }
    if (matches.size() > 3 && matches[3].matched) {
      m_text = matches[3].str() + tail;
    }
    return {};
  } else {
    return std::unexpected("Invalid response format");
  }
}

void SMTPResponse::FormatResponse(const std::string &response) {
  m_formated_response = "S: " + response;
  m_formated_response =
      std::regex_replace(m_formated_response, std::regex("\n"), "\nS: ");
  m_formated_response.erase(m_formated_response.end() - 3,
                            m_formated_response.end());
}

bool SMTPResponse::IsValidResponse(const std::string &response,
                                   std::smatch &matches) const {
  static const std::regex responsePattern(
      R"(^(\d{3})(?:[ -](\d\.\d\.\d))?[ -](.*)$)");

  return std::regex_match(response, matches, responsePattern);
}

auto SMTPResponse::SplitAtEndline(const std::string &response) const
    -> std::pair<std::string, std::string> {
  auto endline_pos = std::min(response.find("\n"), response.find("\r"));
  return std::make_pair(
      response.substr(0, endline_pos),
      response.substr(std::min(endline_pos, response.size())));
}

bool SMTPResponse::CodeEquals(u_int16_t code) const { return m_code == code; }

bool SMTPResponse::StatusEquals(StatusType status) const {
  return m_status == status;
}

}  // namespace ISXResponse