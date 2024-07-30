#include "SmtpClient.h"
#include "SmartSocket.h"

using boost::asio::ip::tcp;
using std::string;
using namespace boost;

namespace ISXSC
{
    SmtpClient::SmtpClient(
        asio::io_context& io_context
        , asio::ssl::context& ssl_context):
        m_smart_socket(io_context, ssl_context) {};

    SmtpClient::~SmtpClient()
    {
        Dispose();
    };

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
        
        // Sending authentication query
        m_smart_socket.Write(query);

        // Formatting response, so that line received from server starts with "S: "
        string response = m_smart_socket.Read();
        std::cout << response;

        if (response.find("235") != string::npos)
        {
            return true;
        };
        
        std::cerr << "Error authenticating: " << response << std::endl;
        return false;
    };

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
}; // namespace ISXSC