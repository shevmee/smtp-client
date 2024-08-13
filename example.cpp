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
            ISXResponse::SMTPResponse response = socket.AsyncReadCoroutine(yield);
            std::cout << response.get_formated_response();
            ISXResponse::SMTPResponse::CheckStatus(response, ISXResponse::StatusType::PositiveCompletion);
            
            socket.AsyncWriteCoroutine("EHLO lorem ipsum\r\n", yield);
            response = socket.AsyncReadCoroutine(yield);
            std::cout << response.get_formated_response();
            ISXResponse::SMTPResponse::CheckStatus(response, ISXResponse::StatusType::PositiveCompletion);

            socket.AsyncWriteCoroutine("QUIT\r\n", yield);
            response = socket.AsyncReadCoroutine(yield);
            std::cout << response.get_formated_response();
            ISXResponse::SMTPResponse::CheckStatus(response, ISXResponse::StatusType::PositiveCompletion);

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
        std::cerr << e.what() << std::endl;
    }

    io_context.stop();
    worker.join();
}