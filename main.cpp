#include <iostream>

#include "SMTPResponse.h"

int main() {
    try {
        ISXR::SMTPResponse response{"250 2.1.5 OK ... - gsmtp  "};

        std::cout << "Code: " << response.get_code() << std::endl;
        std::cout << "Enhanced Status Code: " << response.get_enhanced_code() << std::endl;
        std::cout << "Text: " << response.get_text() << std::endl;
        std::cout << "Formated Response: " << response.get_formated_response() << std::endl;

        if (response.get_status() == ISXR::StatusType::PermanentNegative) 
        {
            // something went wrong
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}