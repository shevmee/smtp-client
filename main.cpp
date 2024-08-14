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
        smtp_client.AsyncAuthenticate("romanbychko84@gmail.com", "azsc ypnd buwv srsy").get();

        ISXMM::MailMessageBuilder mail_builder;
        mail_builder.set_from("romanbychko84@gmail.com", "Roman Buchko")
            .add_to("irabychko84@gmail.com", "Ira Buchko")
            .set_subject("Test message")
            .set_body("howdy, Hello, Ira! This is a test message from Roman.")
            .add_attachment("/media/roma-b/Новий том/Softserve/C++/smtp-client/CMakeLists.txt")
            .add_attachment("/media/roma-b/Новий том/Softserve/C++/smtp-client/main.cpp");

        smtp_client.AsyncSendMail(mail_builder.Build()).get();
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
