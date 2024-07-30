#include <iostream>
#include "SmtpClient.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <SmartSocket.h>

using std::cout;

int main()
{
    boost::asio::io_context io_context;
    
    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tls_client);
    ssl_context.set_default_verify_paths();
    ssl_context.set_verify_mode(boost::asio::ssl::verify_peer);

    ISXSC::SmtpClient smtp_client(io_context, ssl_context);
    smtp_client.Connect("smtp.gmail.com", 587);
    smtp_client.Authenticate("username", "password");

    io_context.run();

    return 0;
}