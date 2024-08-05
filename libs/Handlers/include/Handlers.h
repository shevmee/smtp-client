#pragma once

#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include <iostream>

using std::string;

namespace ISXLOGS
{
    class SmartSocketMethodsHandlers
    {
    public:
        static bool HandleConnection(
            const string& server, const int port
            , const boost::system::error_code& error_code);

        static bool HandleWrite(
            const string& data
            , const boost::system::error_code& error_code);

        static string HandleRead(
            boost::asio::streambuf& buffer
            , const boost::system::error_code& error_code);

        static bool HandleClose(
            const boost::system::error_code& error_code);

        static bool HandleUpgradeSecurity(
            const boost::system::error_code& error_code
            , bool* ssl_toogle);

    private:
        SmartSocketMethodsHandlers() = delete;
        ~SmartSocketMethodsHandlers() = delete;
        
        static inline void HandleError(
            const string& prefix = "ERR", const boost::system::error_code& error_code);

        static string FormatServerOutput(string raw_output);
        static inline std::ostream* s_log_stream = &std::clog;
    };
}; // namespace ISXErrorHandlers