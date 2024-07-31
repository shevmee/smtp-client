#include <iostream>
#include "MailMessageBuilder.h"
#include "MailMessageFormatter.h"

int main()
{
    ISXSC::MailMessageBuilder builder;
    auto message  = builder.SetFrom("romanbychko84@gmail.com", "Roman Buchko")
        .AddTo("irabychko84@GMAIL.COM", "Buchko Ira")
        .SetSubject("Testing...")
        .SetBody("Hello, World \n This is a test message. \n Regards, Roman Buchko.")
        .Build();
    
    std::cout<< ISXSC::MailMessageFormatter::MailFormat(message);
}
