#include "SmtpClient.h"

namespace ISXSC
{
    SmtpClient::SmtpClient(
        asio::io_context& io_context
        , asio::ssl::context& ssl_context):
        m_smart_socket(io_context, ssl_context)
        , m_timeout(S_DEFAULT_TIMEOUT) {};

    SmtpClient::~SmtpClient()
    {
        Dispose();
    };

    // Strand part
    bool SmtpClient::Connect(const string& server, int port)
    {
        m_smart_socket.Connect(server, port);

        std::cout << m_smart_socket.Read();
        if (!m_smart_socket.IsOpen())
        {
            return false;
        };
        
        SendEhloCmd();
        std::cout << m_smart_socket.Read();

        SendStartTlsCmd();
        std::cout << m_smart_socket.Read();

        return UpgradeSecurity();
    };

    bool SmtpClient::Dispose()
    {
        return m_smart_socket.Close();
    };

    bool SmtpClient::Authenticate(const string& username, const string& password)
    {
        m_username = username;
        m_password = password;
        std::string auth_string = '\0' + username + '\0' + password;
        std::string encoded_auth_string = ISXBase64::Base64Encode(auth_string);

        // Forming query
        std::string query = (format("%1% %2% \r\n")
            % S_CMD_AUTH_PLAIN
            % encoded_auth_string).str();
        
        m_smart_socket.Write(query);

        string response = m_smart_socket.Read();
        std::cout << response;

        if (response.find("235") != string::npos)
        {
            return true;
        };
        
        std::cerr << "Error authenticating: " << response << std::endl;
        return false;
    };

    // Async part
    future<void> SmtpClient::AsyncConnect(const string& server, int port)
    {
        std::promise<void> promise;
        future<void> future = promise.get_future();

        asio::spawn(
            m_smart_socket.GetIoContext()
            , [this, server, port, promise = std::move(promise)](asio::yield_context yield)
            mutable
            {
                asio::steady_timer timer(m_smart_socket.GetIoContext());
                SetTimeout(timer, m_timeout, &promise);

                m_smart_socket.AsyncConnectCoroutine(server, port, yield);
                std::cout << m_smart_socket.AsyncReadCoroutine(yield);
            
                AsyncSendEhloCmd(yield);
                std::cout << m_smart_socket.AsyncReadCoroutine(yield);
            
                AsyncSendStartTlsCmd(yield);
                std::cout << m_smart_socket.AsyncReadCoroutine(yield);
            
                AsyncUpgradeSecurity(yield);

                timer.cancel();
                promise.set_value();
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
                asio::steady_timer timer(m_smart_socket.GetIoContext());
                SetTimeout(timer, m_timeout, &promise);

                m_smart_socket.AsyncWriteCoroutine(query, yield);
                std::cout << m_smart_socket.AsyncReadCoroutine(yield);

                timer.cancel();
                promise.set_value();
            }
        );

        return future;
    };

    // Strand part
    bool SmtpClient::SendEhloCmd()
    {
        string query = (format("%1% %2%:%3% \r\n")
            % S_CMD_EHLO
            % m_smart_socket.GetLocalname()
            % m_smart_socket.GetLocalPort()).str();

        return m_smart_socket.Write(query);
    };

    bool SmtpClient::SendStartTlsCmd()
    {
        return m_smart_socket.Write(S_CMD_STARTTLS + "\r\n");
    };

    bool SmtpClient::UpgradeSecurity()
    {
        return m_smart_socket.UpgradeSecurity();
    };

    // Async part
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

    // Timeout timer
    void SmtpClient::SetTimeout(asio::steady_timer& timer, int seconds, std::promise<void>* promise)
    {
        timer.expires_from_now(std::chrono::seconds(seconds));
        timer.async_wait(
            [promise](const boost::system::error_code& ec)
            {
                if (ec)
                {
                    return;
                };

                promise->set_exception(std::make_exception_ptr(std::runtime_error("Timeout waiting for response occurred")));
            }
        );
    };
}; // namespace ISXSC