#include "Base64.hpp"

namespace ISXBase64
{
    // Base64 encoding
    std::string Base64Encode(std::string_view decoded)
    {
        auto encoded_size = boost::beast::detail::base64::encoded_size(decoded.size());
        std::string encoded_output(encoded_size, '\0');
        boost::beast::detail::base64::encode(encoded_output.data(), decoded.data(), decoded.size());
        return encoded_output;
    }

    // Base64 decoding
    std::string Base64Decode(std::string_view encoded)
    {
        auto decoded_size = boost::beast::detail::base64::decoded_size(encoded.size());
        std::string decoded_output(decoded_size, '\0');
        boost::beast::detail::base64::decode(decoded_output.data(), encoded.data(), encoded.size());
        return decoded_output;
    }
} // namespace ISXBase64
