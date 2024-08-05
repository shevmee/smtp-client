#include "SmartSocket.h"
#include "Handlers.h"

#include <iostream>

namespace ISXSmartSocket
{
    SmartSocket::SmartSocket(
        asio::io_context& io_context
        , asio::ssl::context& ssl_context):
        m_io_context(io_context)
        , m_ssl_context(ssl_context)
        , m_resolver(io_context)
        , m_socket(io_context, ssl_context)
        , m_ssl_enabled(false) {};

    SmartSocket::~SmartSocket()
    {
        Close();
    };

    bool SmartSocket::IsOpen() const
    {
        return m_socket.next_layer().is_open();
    };

    string SmartSocket::GetLocalname() const
    {
        return m_socket.next_layer().local_endpoint().address().to_string();
    };

    int SmartSocket::GetLocalPort() const
    {
        return m_socket.next_layer().local_endpoint().port();
    };

    string SmartSocket::GetServername() const
    {
        return m_socket.next_layer().remote_endpoint().address().to_string();
    };

    int SmartSocket::GetServerPort() const
    {
        return m_socket.next_layer().remote_endpoint().port();
    };

    boost::asio::io_context& SmartSocket::GetIoContext()
    {
        return m_io_context;
    };

    bool SmartSocket::AsyncConnectCoroutine(const string& server, int port, asio::yield_context& yield)
    {
        m_server = server;
        m_port = port;

        system::error_code ec;

        tcp::resolver::query query(m_server, std::to_string(m_port));
        tcp::resolver::results_type results = m_resolver.async_resolve(query, yield[ec]);
        asio::async_connect(m_socket.next_layer(), results.begin(), results.end(), yield[ec]);
        
        return ISXLOGS::SmartSocketMethodsHandlers::HandleConnection(GetServername(), GetServerPort(), ec);
    };

    bool SmartSocket::AsyncWriteCoroutine(const string& data, asio::yield_context& yield)
    {
        system::error_code ec;
        
        if (!m_ssl_enabled)
        {
            asio::async_write(m_socket.next_layer(), asio::buffer(data), yield[ec]);
        } else
        {
            asio::async_write(m_socket, asio::buffer(data), yield[ec]);
        };

        return ISXLOGS::SmartSocketMethodsHandlers::HandleWrite(data, ec);
    };

    string SmartSocket::AsyncReadCoroutine(asio::yield_context& yield)
    {
        asio::streambuf buffer;
        system::error_code ec;

        if (!m_ssl_enabled)
        {
            asio::async_read_until(m_socket.next_layer(), buffer, "\r\n", yield[ec]);   
        } else
        {
            asio::async_read_until(m_socket, buffer, "\r\n", yield[ec]);   
        };

        return ISXLOGS::SmartSocketMethodsHandlers::HandleRead(buffer, ec);
    };

    bool SmartSocket::AsyncUpgradeSecurityCoroutine(asio::yield_context& yield)
    {
        system::error_code ec;
        m_socket.async_handshake(boost::asio::ssl::stream_base::handshake_type::client, yield[ec]);
    
        return ISXLOGS::SmartSocketMethodsHandlers::HandleUpgradeSecurity(ec, &m_ssl_enabled);
    };

    bool SmartSocket::Close()
    {
        system::error_code ec;
        if (IsOpen())
        {
            m_socket.next_layer().close(ec);
        } else
        {
            return true;
        };

        return ISXLOGS::SmartSocketMethodsHandlers::HandleClose(ec);
    };
}; // namespace ISXSmartSocket