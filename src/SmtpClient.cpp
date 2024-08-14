#include "SmtpClient.h"
#include "SMTPResponse.h"

namespace ISXSC
{
SmtpClient::SmtpClient(
    asio::io_context& io_context
    , asio::ssl::context& ssl_context):
    m_smart_socket(io_context, ssl_context)
    , m_timeout(S_DEFAULT_TIMEOUT)
{
    m_smart_socket.SetTimeout(S_DEFAULT_TIMEOUT);
};

SmtpClient::~SmtpClient()
{
    Dispose();
};

future<void> SmtpClient::AsyncConnect(const string& server, int port)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    asio::spawn(
        m_smart_socket.GetIoContext()
        , [this, server, port, promise = std::move(promise)](asio::yield_context yield)
        mutable
        {
            try
            {
                m_smart_socket.AsyncConnectCoroutine(server, port, yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket.AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
            
                AsyncSendEhloCmd(yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket.AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
            
                AsyncSendStartTlsCmd(yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket.AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
            
                AsyncUpgradeSecurity(yield);

                promise.set_value();
            } catch (...)
            {
                promise.set_exception(std::current_exception());
            };
        }
    );
    
    return future;
};

future<void> SmtpClient::AsyncAuthenticate(const string& username, const string& password)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    m_username = username;
    m_password = password;
    std::string auth_string = '\0' + username + '\0' + password;
    std::string encoded_auth_string = ISXBase64::Base64Encode(auth_string);

    std::string query = (format("%1% %2% \r\n")
        % S_CMD_AUTH_PLAIN
        % encoded_auth_string).str();

    asio::spawn(
        m_smart_socket.GetIoContext()
        , [this, username, password, query, promise = std::move(promise)](asio::yield_context yield)
        mutable
        {
            try
            {
                m_smart_socket.AsyncWriteCoroutine(query, yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket.AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);

                promise.set_value();
            }
            catch(...)
            {
                promise.set_exception(std::current_exception());
            };
        }
    );

    return future;
};

bool SmtpClient::Dispose()
{
    return m_smart_socket.Close();
};

bool SmtpClient::SetTimeout(int timeout)
{
    return m_smart_socket.SetTimeout(timeout);
};

bool SmtpClient::AsyncSendEhloCmd(asio::yield_context& yield)
{
    string query = (format("%1% %2%:%3% \r\n")
        % S_CMD_EHLO
        % m_smart_socket.GetLocalname()
        % m_smart_socket.GetLocalPort()).str();

    return m_smart_socket.AsyncWriteCoroutine(query, yield);   
};

bool SmtpClient::AsyncSendStartTlsCmd(asio::yield_context& yield)
{
    return m_smart_socket.AsyncWriteCoroutine(S_CMD_STARTTLS + "\r\n", yield);
};

bool SmtpClient::AsyncUpgradeSecurity(asio::yield_context& yield)
{
    return m_smart_socket.AsyncUpgradeSecurityCoroutine(yield);
};
}; // namespace ISXSC