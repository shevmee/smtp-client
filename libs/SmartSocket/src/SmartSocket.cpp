#include "SmartSocket.hpp"

#include <boost/asio/steady_timer.hpp>
#include <iostream>
#include <memory>

#include "Handlers.hpp"

namespace ISXSmartSocket {
SmartSocket::SmartSocket(asio::io_context &io_context,
                         asio::ssl::context &ssl_context)
    : m_io_context(io_context),
      m_ssl_context(ssl_context),
      m_resolver(io_context),
      m_socket(io_context, ssl_context),
      m_ssl_enabled(false){};

SmartSocket::~SmartSocket() {
  try {
    Close();
  } catch (const std::exception &e) {
    std::cerr << "Exception in destructor called" << std::endl;
  }
};

bool SmartSocket::IsOpen() const { return m_socket.next_layer().is_open(); };

string SmartSocket::GetLocalname() const {
  return m_socket.next_layer().local_endpoint().address().to_string();
};

int SmartSocket::GetLocalPort() const {
  return m_socket.next_layer().local_endpoint().port();
};

string SmartSocket::GetServername() const {
  system::error_code ec;
  string server_name =
      m_socket.next_layer().remote_endpoint(ec).address().to_string();
  ISXLogs::SmartSocketMethodsHandlers::HandleRemoteEndpointOp(ec);
  return server_name;
};

int SmartSocket::GetServerPort() const {
  system::error_code ec;
  int server_port = m_socket.next_layer().remote_endpoint(ec).port();
  ISXLogs::SmartSocketMethodsHandlers::HandleRemoteEndpointOp(ec);
  return server_port;
};

asio::io_context &SmartSocket::GetIoContext() { return m_io_context; };

asio::ssl::context &SmartSocket::GetSslContext() { return m_ssl_context; };

bool SmartSocket::SetTimeout(int timeout) {
  m_timeout = timeout;
  return true;
};

bool SmartSocket::AsyncConnectCoroutine(const string &server, int port,
                                        asio::yield_context &yield) {
  m_server = server;
  m_port = port;

  system::error_code ec;

  auto timer = StartTimer(m_timeout, yield, ec);

  tcp::resolver::query query(m_server, std::to_string(m_port));
  tcp::resolver::results_type results =
      m_resolver.async_resolve(query, yield[ec]);
  asio::async_connect(m_socket.next_layer(), results.begin(), results.end(),
                      yield[ec]);

  timer->cancel();

  return ISXLogs::SmartSocketMethodsHandlers::HandleConnection(
      GetServername(), GetServerPort(), ec);
};

bool SmartSocket::AsyncWriteCoroutine(const string &data,
                                      asio::yield_context &yield) {
  system::error_code ec;
  auto data_crlf = data + "\r\n";

  auto timer = StartTimer(m_timeout, yield, ec);

  if (!m_ssl_enabled) {
    asio::async_write(m_socket.next_layer(), asio::buffer(data_crlf),
                      yield[ec]);
  } else {
    asio::async_write(m_socket, asio::buffer(data_crlf), yield[ec]);
  };

  timer->cancel();

  return ISXLogs::SmartSocketMethodsHandlers::HandleWrite(data_crlf, ec);
};

ISXResponse::SMTPResponse SmartSocket::AsyncReadCoroutine(
    asio::yield_context &yield) {
  system::error_code ec;
  asio::streambuf buffer;

  auto timer = StartTimer(m_timeout, yield, ec);

  if (!m_ssl_enabled) {
    asio::async_read_until(m_socket.next_layer(), buffer, "\r\n", yield[ec]);
  } else {
    asio::async_read_until(m_socket, buffer, "\r\n", yield[ec]);
  };

  timer->cancel();

  return ISXLogs::SmartSocketMethodsHandlers::HandleRead(buffer, ec);
};

bool SmartSocket::AsyncUpgradeSecurityCoroutine(asio::yield_context &yield) {
  system::error_code ec;
  m_socket.async_handshake(
      boost::asio::ssl::stream_base::handshake_type::client, yield[ec]);
  m_ssl_enabled = true;
  return ISXLogs::SmartSocketMethodsHandlers::HandleUpgradeSecurity(
      ec, &m_ssl_enabled);
};

bool SmartSocket::Close() {
  system::error_code ec;
  m_socket.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                   ec);
  m_socket.lowest_layer().cancel(ec);
  m_socket.lowest_layer().close(ec);
  return ISXLogs::SmartSocketMethodsHandlers::HandleClose(ec, &m_ssl_enabled);
};

std::unique_ptr<asio::steady_timer> SmartSocket::StartTimer(
    int seconds, asio::yield_context &yield, system::error_code &ec) {
  std::unique_ptr<asio::steady_timer> timer =
      std::make_unique<asio::steady_timer>(yield.get_executor());
  timer->expires_after(std::chrono::seconds(seconds));
  timer->async_wait([&](const system::error_code &error) {
    if (!error) {
      if (!m_ssl_enabled) {
        m_socket.next_layer().cancel();
      } else {
        m_socket.lowest_layer().cancel();
      };
    }
  });

  return timer;
};
};  // namespace ISXSmartSocket