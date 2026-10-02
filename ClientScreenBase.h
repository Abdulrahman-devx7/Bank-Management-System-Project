#pragma once
#include "IScreen.h"
#include "Client.h"
#include <string>

class ClientScreenBase : protected IScreen 
{
protected:

    ClientScreenBase() {}

    static string ReadAccountNumber()
    {
        string userInput = "";
        getline(cin >> ws, userInput);

        return userInput;
    }

    static void PrintInfoCard(Client& client)
    {
        const int indentWidth = 40;
        const int labelWidth = 18;
        const std::string indent(indentWidth, ' ');

        std::cout << "\n" << indent << UI_LINE_BOUNDS << "\n";
        std::cout << indent << std::setw(labelWidth + 12) << "Client Card Details" << "\n";
        std::cout << indent << UI_LINE_BOUNDS << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Account Number"
            << ": " << client.account().AccountNumber() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "PIN Code"
            << ": " << client.account().PIN() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Full Name"
            << ": " << client.FullName() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Email"
            << ": " << client.Email() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Phone Number"
            << ": " << client.PhoneNumber() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Account Balance"
            << ": $" << std::fixed << std::setprecision(2) << client.account().Balance() << "\n";

        std::cout << indent << UI_LINE_BOUNDS << "\n\n";
    }

};

