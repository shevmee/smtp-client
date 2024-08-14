#include "SmtpClient.h"
#include "SmartSocket.h"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <SmartSocket.h>

using std::cout;

int main()
{
    // Initialize the io_context and ssl_context
    boost::asio::io_context io_context;
    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tls_client);
    
    // Make io_context run in a separate thread and event loop poll for new async operations
    std::thread worker([&io_context]() {
        asio::io_context::work work(io_context);
        io_context.run();
    });

    ISXSC::SmtpClient smtp_client(io_context, ssl_context);

    try
    {
        smtp_client.AsyncConnect("smtp.gmail.com", 587).get();
        smtp_client.AsyncAuthenticate("username", "password").get();
        smtp_client.Dispose();
    } catch (const std::exception& e)
    {
        // process exception
        
        io_context.stop();
        worker.join();
        return 1;
    };
    

    io_context.stop();
    worker.join();
    return 0;
}