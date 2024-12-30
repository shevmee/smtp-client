#include "Handlers.hpp"

#include <format>

namespace ISXLogs {
bool SmartSocketMethodsHandlers::HandleConnection(
    const string &server, const int port,
    const boost::system::error_code &error_code) {
  if (!error_code) {
    *s_log_stream << std::format("Log: Successfully connected to {}:{}", server,
                                 port)
                  << std::endl;
    return true;
  }

  logError("Connection error", error_code);
  return false;
};

bool SmartSocketMethodsHandlers::HandleRemoteEndpointOp(
    const boost::system::error_code &error_code) {
  if (!error_code) {
    return true;
  }

  logError("RemoteEndpoint error", error_code);
  return false;
};

bool SmartSocketMethodsHandlers::HandleWrite(
    const string &data, const boost::system::error_code &error_code) {
  if (!error_code) {
    *s_log_stream << std::format("C: {}", data) << std::endl;
    return true;
  }

  handleTimeout(error_code);
  logError("Write error", error_code);
  return false;
};

ISXResponse::SMTPResponse SmartSocketMethodsHandlers::HandleRead(
    boost::asio::streambuf &buffer,
    const boost::system::error_code &error_code) {
  if (error_code && error_code != boost::asio::error::operation_aborted) {
    logError("Reading error", error_code);
  } else if (error_code == boost::asio::error::operation_aborted) {
    logError("Reading error", boost::asio::error::timed_out);
  };

  if (!error_code) {
    std::stringstream response;
    std::copy(boost::asio::buffers_begin(buffer.data()),
              boost::asio::buffers_end(buffer.data()),
              std::ostream_iterator<char>(response));

    ISXResponse::SMTPResponse smtp_response(response.str());
    *s_log_stream << smtp_response.get_formated_response();

    return smtp_response;
  };

  handleTimeout(error_code);
  logError("Reading error", error_code);
  return ISXResponse::SMTPResponse("");
};

bool SmartSocketMethodsHandlers::HandleClose(
    const boost::system::error_code &error_code, bool *ssl_toggle) {
  if (!error_code) {
    *s_log_stream << "Log: Connection closed" << std::endl;
    *ssl_toggle = false;
    return true;
  }

  logError("Close error", error_code);
  return false;
};

bool SmartSocketMethodsHandlers::HandleUpgradeSecurity(
    const boost::system::error_code &error_code, bool *ssl_toggle) {
  if (!error_code) {
    *s_log_stream << "Log: Handshake successful. Connection upgraded to TLS"
                  << std::endl;
    *ssl_toggle = true;
    return true;
  }

  *ssl_toggle = false;
  logError("Update security error", error_code);
  return false;
};
};  // namespace ISXLogs