#pragma once

#include <boost/beast/core/detail/base64.hpp>
#include <string_view>

using std::string;

namespace ISXBase64
{
    string Base64Encode(std::string_view decoded);

    string Base64Decode(std::string_view encoded);
}; // namespace ISXBase64