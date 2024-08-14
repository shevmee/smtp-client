#include "Handlers.h"

#include <regex>

namespace ISXLogs
{
void SmartSocketMethodsHandlers::HandleError(
    const string& prefix, const boost::system::error_code& error_code)
{
    *s_log_stream << prefix << ": " << error_code.message() << std::endl;
    throw std::runtime_error(error_code.message());
};

bool SmartSocketMethodsHandlers::HandleConnection(
        const string& server, const int port
        , const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        *s_log_stream << "LOG: " << "Successfully connected to " << server << ":" << port << std::endl;
        return true;
    }

    HandleError("Connection error", error_code);
    return false;
};

bool SmartSocketMethodsHandlers::HandleWrite(
        const string& data
        , const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        *s_log_stream << "C: " << data;
        return true;
    }

    HandleError("Write error: ", error_code);
    return false;
};

ISXResponse::SMTPResponse SmartSocketMethodsHandlers::HandleRead(
        boost::asio::streambuf& buffer
        , const boost::system::error_code& error_code)
{
    if (error_code && error_code != boost::asio::error::operation_aborted)
    {
        HandleError("Reading error", error_code);
    } else if (error_code == boost::asio::error::operation_aborted)
    {
        HandleError("Reading error", boost::asio::error::timed_out);
    };

    std::stringstream response;
    std::copy(
        boost::asio::buffers_begin(buffer.data()),
        boost::asio::buffers_end(buffer.data()),
        std::ostream_iterator<char>(response)
    );
    
    ISXResponse::SMTPResponse smtp_response(response.str());
    *s_log_stream << smtp_response.get_formated_response();

    return smtp_response;
};

bool SmartSocketMethodsHandlers::HandleClose(
        const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        *s_log_stream << "LOG: " << "Connection closed" << std::endl;
        return true;
    }

    HandleError("Close error", error_code);
    return false;
};

bool SmartSocketMethodsHandlers::HandleUpgradeSecurity(
        const boost::system::error_code& error_code
        , bool* ssl_toggle)
{
    if (!error_code)
    {
        *s_log_stream << "LOG: " << "Handshake successful. Connection upgraded to TLS" << std::endl;
        *ssl_toggle = true;
        return true;
    }

    HandleError("Update security error", error_code);
    *ssl_toggle = false;
    return false;
};
}; // namespace ISXLogs