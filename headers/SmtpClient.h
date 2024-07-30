#pragma once

#include "SmartSocket.h"
#include "Base64.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <string>

using std::string;
using std::unique_ptr;
using std::string;
using std::future;

using boost::asio::ip::tcp;

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

        // smtp with starttls extension uses 587 port
        SmtpClient(
            boost::asio::io_context& io_context
            , boost::asio::ssl::context& ssl_context);

        ~SmtpClient();

        bool Connect(const string& server, int port);
        bool Dispose();

        bool Authenticate(const string& username, const string& password);

    private:

        ISXSmartSocket::SmartSocket m_smart_socket;

        string m_username;
        string m_password;
        

        bool SendEhloCmd();
        bool SendStartTlsCmd();

        bool UpgradeSecurity();

        bool SendMailFromCmd();
        bool SendRcptToCmd();
        bool SendDataCmd();
        bool SendQuitCmd();
    };
}; // namespace ISXSC