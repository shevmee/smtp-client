#include "Handlers.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>

int main()
{
    boost::asio::streambuf response;
    response.sputn("220 smtp.example.com ESMTP Postfix", 33);   
    std::cout << ISXLOGS::SmartSocketMethodsHandlers::HandleRead(response, boost::system::error_code()).get_code() << std::endl;
    std::cout << ISXLOGS::SmartSocketMethodsHandlers::HandleRead(response, boost::system::error_code()).get_formated_response() << std::endl;

    try
    {
        ISXLOGS::SmartSocketMethodsHandlers::HandleRead(response, boost::asio::error::operation_aborted);
    }
    catch(const std::exception& e)
    {
        // Process the exception
    }
}