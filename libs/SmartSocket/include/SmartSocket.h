#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <string>
#include <sstream>
#include <regex>

using namespace boost;

using std::string;
using asio::ip::tcp;
using std::function;

namespace ISXSmartSocket
{
    class SmartSocket
    {
    public:
        SmartSocket(
            asio::io_context& m_io_context
            , asio::ssl::context& m_ssl_context);
        ~SmartSocket();

        bool Connect(const string& server, int port);
        
        bool Write(const string& data);
        string Read();
        
        bool Shutdown();
        bool Close();

        bool UpgradeSecurity();

        bool IsOpen() const;
        string GetLocalname() const;
        int GetLocalPort() const;
        string GetServername() const;
        int GetServerPort() const;

    private:
        string m_server;
        int m_port;

        bool m_ssl_enabled;

        tcp::resolver m_resolver;
        boost::asio::io_context& m_io_context;
        boost::asio::ssl::context& m_ssl_context;
        boost::asio::ssl::stream<tcp::socket> m_socket;
    };
}; // namespace ISXSmartSocket