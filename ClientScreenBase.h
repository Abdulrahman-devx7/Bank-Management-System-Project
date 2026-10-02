#pragma once
#include "IScreen.h"
#include "Client.h"
#include "RichString.h"
#include "InputValidate.h"
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

    void ReadFullName(Client& client)
    {
        bool is4Names = true;
        bool isAllLetters = true;
        vector<string> Words;

        do
        {
            string fullName = "";
            getline(cin, fullName);
            client.SetFullName(fullName);

            Words = RichString::Split(client.FullName(), " ");

            is4Names = (Words.size() == 4);
            isAllLetters = InputValidate::AreNamesOnlyLetters(Words);

            if (!is4Names || !isAllLetters)
                cout << "\nPlease, enter a valid full name form consisting of 4 names without any number or special characters!\n\n";

        } while (!isAllLetters || !is4Names);

        client.SetFullName(RichString::JoinString(Words, " "));
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

