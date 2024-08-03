#pragma once

#include "SmartSocket.h"
#include "Base64.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/format.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>

#include <iostream>
#include <string>

using namespace boost;
using std::string;
using std::unique_ptr;
using std::string;
using std::future;

using asio::ip::tcp;

namespace ISXSC
{
    class SmtpClient
    {
    public:
        inline static const string S_CMD_EHLO = "EHLO";
        inline static const string S_CMD_STARTTLS = "STARTTLS";
        inline static const string S_CMD_AUTH_PLAIN = "AUTH PLAIN";
        inline static const string S_CMD_MAIL_FROM = "MAIL FROM";
        inline static const string S_CMD_RCPT_TO = "MAIL TO";
        inline static const string S_CMD_DATA = "DATA";
        inline static const string S_CMD_QUIT = "QUIT";

        inline static const int S_DEFAULT_TIMEOUT = 5;

        SmtpClient(asio::io_context& io_context, asio::ssl::context& ssl_context);
        ~SmtpClient();

        // Strand part
        bool Connect(const string& server, int port);
        bool Authenticate(const string& username, const string& password);
        bool Dispose();

        // Async part
        future<void> AsyncConnect(const string& server, int port);
        future<void> AsyncAuthenticate(const string& username, const string& password);

    private:

        ISXSmartSocket::SmartSocket m_smart_socket;

        string m_username;
        string m_password;

        int m_timeout;

        // Strand part
        bool SendEhloCmd();
        bool SendStartTlsCmd();
        bool UpgradeSecurity();
        bool SendMailFromCmd();
        bool SendRcptToCmd();
        bool SendDataCmd();
        bool SendQuitCmd();

        // Async part
        bool AsyncSendEhloCmd(asio::yield_context& yield);
        bool AsyncSendStartTlsCmd(asio::yield_context& yield);
        bool AsyncUpgradeSecurity(asio::yield_context& yield);

        // Timeout timer
        void SetTimeot(asio::steady_timer& timer, int seconds, std::promise<void>* promise);
    };
}; // namespace ISXSC