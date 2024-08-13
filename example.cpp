#include "SmartSocket.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/spawn.hpp>

int main()
{
    boost::asio::io_context io_context;
    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tls_client);
    ssl_context.set_default_verify_paths();

    std::thread worker([&io_context]() {
        asio::io_context::work work(io_context);
        io_context.run();
    });

    ISXSmartSocket::SmartSocket socket(io_context, ssl_context);

    std::promise<void> promise;
    std::future<void> future = promise.get_future();

    boost::asio::spawn(socket.GetIoContext(), [&socket, &promise](boost::asio::yield_context yield)
    {
        try
        {
            socket.AsyncConnectCoroutine("smtp.gmail.com", 587, yield);
            std::cout << socket.AsyncReadCoroutine(yield).get_formated_response();
            
            socket.AsyncWriteCoroutine("EHLO qwe\r\n", yield);
            std::cout << socket.AsyncReadCoroutine(yield).get_formated_response();

            socket.AsyncWriteCoroutine("QUIT\r\n", yield);
            std::cout << socket.AsyncReadCoroutine(yield).get_formated_response();

            promise.set_value();
        } catch (const std::exception& err)
        {
            promise.set_exception(std::current_exception());
        }
    });

    try
    {
        future.get();
    } catch (const std::exception& e)
    {
        // Process timeout or other errors
    }

    io_context.stop();
    worker.join();
}