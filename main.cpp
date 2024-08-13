#include <iostream>

#include "SMTPResponse.h"

int main() {
    try {
        ISXResponse::SMTPResponse response{R"(250-smtp.gmail.com at your service, [217.65.241.77]
250-SIZE 35882577
250-8BITMIME
250-STARTTLS
250-ENHANCEDSTATUSCODES
250-PIPELINING
250-CHUNKING
250 SMTPUTF8)"};
        
        std::cout << "Code: " << response.get_code() << std::endl;
        std::cout << "Enhanced Status Code: " << response.get_enhanced_code() << std::endl;
        std::cout << "Text: " << response.get_text() << std::endl;
        std::cout << "Formated Response: " << response.get_formated_response() << std::endl;

        if (response.get_status() == ISXResponse::StatusType::PermanentNegative) 
        {
            // something went wrong
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}