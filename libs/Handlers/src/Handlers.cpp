#include "Handlers.h"

#include <regex>

namespace ISXLOGS
{
    void SmartSocketMethodsHandlers::HandleError(
        const string& prefix, const boost::system::error_code& error_code)
    {
        *s_log_stream << prefix << ": " << error_code.message() << std::endl;
        throw std::runtime_error(error_code.message());
    };

    string SmartSocketMethodsHandlers::FormatServerOutput(string raw_output)
    {
        // Formatting response, so that each line received from server starts with "S: "
        raw_output = std::regex_replace(raw_output, std::regex("\n"), "\nS: ");
        raw_output.erase(raw_output.end() - 3, raw_output.end());
        return raw_output;
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

    string SmartSocketMethodsHandlers::HandleRead(
            boost::asio::streambuf& buffer
            , const boost::system::error_code& error_code)
    {
        if (!error_code)
        {
            std::stringstream response;
            response << "S: ";
            std::copy(
                boost::asio::buffers_begin(buffer.data()),
                boost::asio::buffers_end(buffer.data()),
                std::ostream_iterator<char>(response)
            );

            return FormatServerOutput(response.str());
        } else if (error_code == boost::asio::error::operation_aborted)
        {
            HandleError("Reading error", boost::asio::error::timed_out);
            return string();
        };

        HandleError("Reading error", error_code);
        return string();
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
            , bool* ssl_toogle)
    {
        if (!error_code)
        {
            *s_log_stream << "LOG: " << "Handshake successful. Connection upgraded to TLS" << std::endl;
            *ssl_toogle = true;
            return true;
        }

        HandleError("Update security error", error_code);
        *ssl_toogle = false;
        return false;
    };

}; // namespace ISXErrorHandlers