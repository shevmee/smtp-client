#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/spawn.hpp>

#include <string>
#include <sstream>
#include <regex>
#include <future>

using namespace boost;

using std::string;
using asio::ip::tcp;
using std::function;

namespace ISXSmartSocket
{
    class SmartSocket
    {
    public:
        SmartSocket(asio::io_context& io_context, asio::ssl::context& ssl_context);
        ~SmartSocket();

        // Strand part
        bool Connect(const string& server, int port);
        
        bool Write(const string& data);
        string Read(bool raw_output = false);
        
        bool Close();

        bool UpgradeSecurity();

        bool IsOpen() const;
        string GetLocalname() const;
        int GetLocalPort() const;
        string GetServername() const;
        int GetServerPort() const;

        boost::asio::io_context& GetIoContext();

        // Async part
        bool AsyncConnectCoroutine(const string& server, int port, asio::yield_context& yield);
        bool AsyncWriteCoroutine(const string& data, asio::yield_context& yield);
        string AsyncReadCoroutine(asio::yield_context& yield, bool raw_output = false);
        bool AsyncUpgradeSecurityCoroutine(asio::yield_context& yield);

    private:
        string m_server;
        int m_port;

        bool m_ssl_enabled;

        tcp::resolver m_resolver;
        asio::io_context& m_io_context;
        asio::ssl::context& m_ssl_context;
        asio::ssl::stream<tcp::socket> m_socket;
    };
}; // namespace ISXSmartSocket