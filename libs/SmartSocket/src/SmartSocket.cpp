#include "SmartSocket.h"

#include <iostream>
#include <boost/asio/spawn.hpp>

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
        Shutdown();
        Close();
    };

    // Strand part
    bool SmartSocket::Connect(const string& server, int port)
    {
        m_server = server;
        m_port = port;

        system::error_code ec;

        tcp::resolver::query query(m_server, std::to_string(m_port));
        tcp::resolver::results_type results = m_resolver.resolve(query);
        asio::connect(m_socket.next_layer(), results, ec);
        
        if (!ec)
        {
            std::cout << "C: Connected to " << GetServername() << ":" << GetServerPort() << std::endl;
            return true;
        };

        std::cout << "Error: " << ec.message() << std::endl;
        return false;
    };

    bool SmartSocket::Write(const string& data)
    {
        system::error_code ec;
        if (!m_ssl_enabled)
        {
            asio::write(m_socket.next_layer(), asio::buffer(data), ec);
        } else
        {
            asio::write(m_socket, asio::buffer(data), ec);
        };

        if (!ec)
        {
            std::cout << "C: " << data;
            return true;
        };
        
        std::cerr << "Error sending: " << ec.message() << std::endl;
        return false;
    };

    string SmartSocket::Read(bool raw_output)
    {
        asio::streambuf buffer;
        system::error_code ec;

        if (!m_ssl_enabled)
        {
            int bytes = asio::read_until(m_socket.next_layer(), buffer, "\r\n", ec);
        }
        else
        {
            int bytes = asio::read_until(m_socket, buffer, "\r\n", ec);
        };

        if (!ec)
        {
            std::stringstream response;
            response << "S: ";
            std::copy(
                asio::buffers_begin(buffer.data()),
                asio::buffers_end(buffer.data()),
                std::ostream_iterator<char>(response)
            );

            if (raw_output)
            {
                return response.str();
            };

            return FormatServerOutput(response.str());
        };

        std::cerr << "Error receiving: " << ec.message() << std::endl;
        return string();
    }

    bool SmartSocket::Shutdown()
    {
        system::error_code ec;
    
        if (IsOpen())
        {
            if (!m_ssl_enabled)
            {
                m_socket.next_layer().shutdown(tcp::socket::shutdown_both, ec);
            } else
            {
                m_socket.shutdown(ec);
            };
        } else
        {
            return true;
        }
    
        if (!ec)
        {
            return true;
        };
    
        std::cerr << "Error shutdown: " << ec.message() << std::endl;
        return false;
    };

    bool SmartSocket::Close()
    {
        system::error_code ec;
        m_socket.next_layer().close(ec);

        if (!ec)
        {
            return true;
        };

        std::cerr << "Error closing: " << ec.message() << std::endl;
        return false;
    };

    bool SmartSocket::UpgradeSecurity()
    {
        system::error_code ec;
        m_socket.handshake(boost::asio::ssl::stream_base::handshake_type::client, ec);
    
        if (!ec)
        {
            std::cout << "C: Handshake successful. Connection upgraded to TLS" << std::endl;
            m_ssl_enabled = true;
            return true;
        };
        
        std::cerr << "Error during handshake: " << ec.message() << std::endl;
        return false;
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

    string SmartSocket::FormatServerOutput(string raw_output)
    {
        // Formatting response, so that each line received from server starts with "S: "
        raw_output = std::regex_replace(raw_output, std::regex("\n"), "\nS: ");
        raw_output.erase(raw_output.end() - 3, raw_output.end());
        return raw_output;
    };

    boost::asio::io_context& SmartSocket::GetIoContext()
    {
        return m_io_context;
    };

    // Async part
    bool SmartSocket::AsyncConnectCoroutine(const string& server, int port, asio::yield_context& yield)
    {
        m_server = server;
        m_port = port;

        system::error_code ec;

        tcp::resolver::query query(m_server, std::to_string(m_port));
        tcp::resolver::results_type results = m_resolver.async_resolve(query, yield[ec]);
        asio::async_connect(m_socket.next_layer(), results.begin(), results.end(), yield[ec]);
        
        if (!ec)
        {
            std::cout << "Connected to " << GetServername() << ":" << GetServerPort() << std::endl;
            return true;
        };

        std::cout << "Error: " << ec.message() << std::endl;
        return false;
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

        if (!ec)
        {
            std::cout << "C: " << data;
            return true;
        };
        
        std::cerr << "Error sending: " << ec.message() << std::endl;
        return false;
    };

    string SmartSocket::AsyncReadCoroutine(asio::yield_context& yield, bool raw_output)
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

        if (!ec)
        {
            std::stringstream response;
            response << "S: ";
            std::copy(
                asio::buffers_begin(buffer.data()),
                asio::buffers_end(buffer.data()),
                std::ostream_iterator<char>(response)
            );

            if (raw_output)
            {
                return response.str();
            };

            return FormatServerOutput(response.str());
        };

        std::cerr << "Error receiving: " << ec.message() << std::endl;
        return string();
    };

    bool SmartSocket::AsyncUpgradeSecurityCoroutine(asio::yield_context& yield)
    {
        system::error_code ec;
        m_socket.async_handshake(boost::asio::ssl::stream_base::handshake_type::client, yield[ec]);
    
        if (!ec)
        {
            std::cout << "C: Handshake successful. Connection upgraded to TLS" << std::endl;
            m_ssl_enabled = true;
            return true;
        };
        
        std::cerr << "Error during handshake: " << ec.message() << std::endl;
        return false;
    };
}; // namespace ISXSmartSocket