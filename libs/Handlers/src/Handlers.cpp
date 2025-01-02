#include "Handlers.hpp"

#include <boost/asio.hpp>
#include <format>
#include <iterator>
#include <sstream>
#include <string>

namespace ISXLogs {

std::expected<void, std::string> SmartSocketMethodsHandlers::HandleConnection(
    const std::string &server, const int port,
    const boost::system::error_code &error_code) {
  if (!error_code) {
    *s_log_stream << std::format("Log: Successfully connected to {}:{}", server,
                                 port)
                  << std::endl;
    return {};
  }

  logError("Connection error", error_code);
  return std::unexpected("Connection error");
}

std::expected<void, std::string>
SmartSocketMethodsHandlers::HandleRemoteEndpointOp(
    const boost::system::error_code &error_code) {
  if (!error_code) {
    return {};
  }

  logError("RemoteEndpoint error", error_code);
  return std::unexpected("RemoteEndpoint error");
}

std::expected<void, std::string> SmartSocketMethodsHandlers::HandleWrite(
    const std::string &data, const boost::system::error_code &error_code) {
  if (!error_code) {
    *s_log_stream << std::format("C: {}", data) << std::endl;
    return {};
  }

  handleTimeout(error_code);
  logError("Write error", error_code);
  return std::unexpected("Write error");
}

std::expected<ISXResponse::SMTPResponse, std::string>
SmartSocketMethodsHandlers::HandleRead(
    boost::asio::streambuf &buffer,
    const boost::system::error_code &error_code) {
  if (error_code) {
    if (error_code == boost::asio::error::operation_aborted) {
      logError("Reading error", boost::asio::error::timed_out);
    } else {
      logError("Reading error", error_code);
    }
    handleTimeout(error_code);
    return std::unexpected("Reading error");
  }

  std::stringstream response;
  std::copy(boost::asio::buffers_begin(buffer.data()),
            boost::asio::buffers_end(buffer.data()),
            std::ostream_iterator<char>(response));

  ISXResponse::SMTPResponse smtp_response(response.str());
  *s_log_stream << smtp_response.get_formated_response();

  return smtp_response;
}

std::expected<void, std::string> SmartSocketMethodsHandlers::HandleClose(
    const boost::system::error_code &error_code, bool *ssl_toggle) {
  if (!error_code) {
    *s_log_stream << "Log: Connection closed" << std::endl;
    *ssl_toggle = false;
    return {};
  }

  logError("Close error", error_code);
  return std::unexpected("Close error");
}

std::expected<void, std::string>
SmartSocketMethodsHandlers::HandleUpgradeSecurity(
    const boost::system::error_code &error_code, bool *ssl_toggle) {
  if (!error_code) {
    *s_log_stream << "Log: Handshake successful. Connection upgraded to TLS"
                  << std::endl;
    *ssl_toggle = true;
    return {};
  }

  *ssl_toggle = false;
  logError("Update security error", error_code);
  return std::unexpected("Update security error");
}

}  // namespace ISXLogs