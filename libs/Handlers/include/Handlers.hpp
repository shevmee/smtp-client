#pragma once

#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <functional>

#include "SMTPResponse.hpp"

using std::string;

namespace ISXLogs
{
class SmartSocketMethodsHandlers
{
public:
    static bool HandleConnection(
        const string& server, const int port
        , const boost::system::error_code& error_code);

    static bool HandleRemoteEndpointOp(
        const boost::system::error_code& error_code);

    static bool HandleWrite(
        const string& data
        , const boost::system::error_code& error_code);

    static ISXResponse::SMTPResponse HandleRead(
        boost::asio::streambuf& buffer
        , const boost::system::error_code& error_code);

    static bool HandleClose(
        const boost::system::error_code& error_code
        , bool* ssl_toggle);

    static bool HandleUpgradeSecurity(
        const boost::system::error_code& error_code
        , bool* ssl_toggle);

private:
    static inline std::ostream* s_log_stream = &std::clog;

    static inline std::function<void(const std::string&, const boost::system::error_code&)> logError = 
        [](const std::string& prefix, const boost::system::error_code& error_code) {
            *s_log_stream << std::format("{}: Error Code {}: {}", prefix, error_code.value(), error_code.message()) << std::endl;
        };

    static inline std::function<void(const boost::system::error_code&)> handleTimeout =
        [](const boost::system::error_code& error_code) {
            if (error_code == boost::asio::error::operation_aborted) {
                *s_log_stream << "Log: Timeout maybe reached" << std::endl;
            }
        };
        
    SmartSocketMethodsHandlers() = delete;
    ~SmartSocketMethodsHandlers() = delete;
};
}; // namespace ISXLogs