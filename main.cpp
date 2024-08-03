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
    ssl_context.set_default_verify_paths();
    ssl_context.set_verify_mode(boost::asio::ssl::verify_peer);

    // Make io_context run in a separate thread and event loop poll for new async operations
    std::thread worker([&io_context]() {
        asio::io_context::work work(io_context);
        io_context.run();
    });

    ISXSC::SmtpClient smtp_client(io_context, ssl_context);

    // Example of async connect to smtp server
    future<void> connection = smtp_client.AsyncConnect("smtp.gmail.com", 587);

    try
    {
        connection.get();
    } catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    smtp_client.AsyncAuthenticate("username", "password").get();

    // Stop the io_context to finish processing
    io_context.stop();
    
    // Wait for the worker thread to finish
    worker.join();

    return 0;
}