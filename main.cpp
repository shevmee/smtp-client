#include "SmtpClient.h"
#include "SmartSocket.h"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <SmartSocket.h>

#include <memory>

using std::cout;
void finish(asio::io_context& io_context, std::thread& worker, std::unique_ptr<ISXSC::SmtpClient>& smtp_client);
 
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

    std::unique_ptr<ISXSC::SmtpClient> smtp_client = std::make_unique<ISXSC::SmtpClient>(io_context, ssl_context);

    try
    {
        smtp_client->AsyncConnect("smtp.gmail.com", 587).get();
        smtp_client->AsyncAuthenticate("user@gmail.com", "password").get();
    } catch (const std::exception& e)
    {
        // process exception

        finish(io_context, worker, smtp_client);
        return 1;
    };
    
    finish(io_context, worker, smtp_client);
    return 0;
}

void finish(asio::io_context& io_context, std::thread& worker, std::unique_ptr<ISXSC::SmtpClient>& smtp_client)
{
    delete smtp_client.release();
    
    if (!io_context.stopped())
        io_context.stop();

    if (worker.joinable())
        worker.join();
}