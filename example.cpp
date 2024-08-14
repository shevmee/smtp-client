#include "Handlers.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>

int main()
{
    boost::asio::streambuf response;
    response.sputn("220 smtp.example.com ESMTP Postfix", 33);   
    
    std::cout
    << "Code: "
    << ISXLogs::SmartSocketMethodsHandlers::HandleRead(response, boost::system::error_code()).get_code()
    << std::endl;

    try
    {
        ISXLogs::SmartSocketMethodsHandlers::HandleRead(response, boost::asio::error::operation_aborted);
    }
    catch(const std::exception& e)
    {
        // Process the exception
    }
}