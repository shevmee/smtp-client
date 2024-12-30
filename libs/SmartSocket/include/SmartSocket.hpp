#pragma once

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl.hpp>
#include <future>
#include <regex>
#include <sstream>
#include <string>

#include "SMTPResponse.hpp"

using namespace boost;

using asio::ip::tcp;
using std::function;
using std::string;

namespace ISXSmartSocket {
class SmartSocket {
 public:
  SmartSocket(asio::io_context &io_context, asio::ssl::context &ssl_context);
  ~SmartSocket();

  bool IsOpen() const;
  string GetLocalname() const;
  int GetLocalPort() const;
  string GetServername() const;
  int GetServerPort() const;

  bool SetTimeout(int timeout);

  asio::io_context &GetIoContext();
  asio::ssl::context &GetSslContext();

  bool AsyncConnectCoroutine(const string &server, int port,
                             asio::yield_context &yield);
  bool AsyncWriteCoroutine(const string &data, asio::yield_context &yield);
  ISXResponse::SMTPResponse AsyncReadCoroutine(asio::yield_context &yield);
  bool AsyncUpgradeSecurityCoroutine(asio::yield_context &yield);
  bool Close();

 private:
  std::unique_ptr<asio::steady_timer> StartTimer(int seconds,
                                                 asio::yield_context &yield,
                                                 system::error_code &ec);

  string m_server;
  int m_port;

  bool m_ssl_enabled;

  int m_timeout = 5;

  tcp::resolver m_resolver;
  asio::io_context &m_io_context;
  asio::ssl::context &m_ssl_context;
  asio::ssl::stream<tcp::socket> m_socket;
};
};  // namespace ISXSmartSocket