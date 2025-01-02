#pragma once

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <expected>
#include <iostream>

#include "SMTPResponse.hpp"

using std::string;

namespace ISXLogs {
class SmartSocketMethodsHandlers {
 public:
  static std::expected<void, std::string> HandleConnection(
      const string &server, const int port,
      const boost::system::error_code &error_code);

  static std::expected<void, std::string> HandleRemoteEndpointOp(
      const boost::system::error_code &error_code);

  static std::expected<void, std::string> HandleWrite(
      const string &data, const boost::system::error_code &error_code);

  static std::expected<ISXResponse::SMTPResponse, std::string> HandleRead(
      boost::asio::streambuf &buffer,
      const boost::system::error_code &error_code);

  static std::expected<void, std::string> HandleClose(
      const boost::system::error_code &error_code, bool *ssl_toggle);

  static std::expected<void, std::string> HandleUpgradeSecurity(
      const boost::system::error_code &error_code, bool *ssl_toggle);

 private:
  static inline std::ostream *s_log_stream = &std::clog;

  static inline auto logError =
      [](const std::string &prefix,
         const boost::system::error_code &error_code) {
        *s_log_stream << std::format("{}: Error Code {}: {}", prefix,
                                     error_code.value(), error_code.message())
                      << std::endl;
      };

  static inline auto handleTimeout =
      [](const boost::system::error_code &error_code) {
        if (error_code == boost::asio::error::operation_aborted) {
          *s_log_stream << "Log: Timeout maybe reached" << std::endl;
        }
      };

  SmartSocketMethodsHandlers() = delete;
  ~SmartSocketMethodsHandlers() = delete;
};
};  // namespace ISXLogs