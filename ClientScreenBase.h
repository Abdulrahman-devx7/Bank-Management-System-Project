#pragma once
#include "IScreen.h"
#include "Client.h"
#include "RichString.h"
#include "InputValidate.h"
#include <string>

class ClientScreenBase : public IScreen 
{
private:
    static string _readAccountNumber()
    {
        string userInput = "";
        getline(cin >> ws, userInput);

        return userInput;
    }

    static void _HandleBadAccNumber(bool isValid)
    {
        if (isValid)
            cout << "\nInput rejected! Please, enter an account number that follows the form: ABC1234 \n";
    }

protected:

    ClientScreenBase() {}

    // Should be merely concerned about reading the raw input and validate it.
    string ReadAccountNumber()
    {
        bool isAccNumberValid = true;
        string input;
        do
        {
            input = _readAccountNumber();
            isAccNumberValid = InputValidate::IsValidAccountNumber(input);
            input = InputValidate::NormalizeAccountNumber(input);

            _HandleBadAccNumber(isAccNumberValid);

        } while (!isAccNumberValid);

        return input;
    }

    string ReadFullName()
    {
        bool is4Names = true;
        bool isAllLetters = true;
        vector<string> Words;

        do
        {
            string fullName = "";
            getline(cin, fullName);

            Words = RichString::Split(fullName, " ");

            is4Names = (Words.size() == 4);
            isAllLetters = InputValidate::AreNamesOnlyLetters(Words);

            if (!is4Names || !isAllLetters)
                cout << "\nPlease, enter a valid full name form consisting of 4 names without any number or special characters!\n\n";

        } while (!isAllLetters || !is4Names);

        return RichString::JoinString(Words, " ");
    }

    string ReadEmail()
    {
        string email = "";
        getline(std::cin >> ws, email);
        return email;
    }

    string ReadPhoneNumber()
    {
        string PhoneNumber = "";
        bool isValid = true;
        do
        {
            getline(cin, PhoneNumber);
            isValid = InputValidate::IsValidPhoneNumber(PhoneNumber);

            if (!isValid)
                cout << "\nPlease enter a valid Egyptian phone number (11 digits, e.g. 01012345678)!\n";
        } while (!isValid);

        return PhoneNumber;
    }

    string ReadPIN()
    {
        string PIN;
        bool isValid = true;
        do
        {
            getline(cin, PIN);
            isValid = InputValidate::IsValidPIN(PIN);

            if (!isValid)
                cout << "\nPlease enter a valid PIN (6 digits, numbers only)\n";

        } while (!isValid);

        return PIN;
    }

    double ReadBalance()
    {
        return InputValidate::ReadFloatBetween(0, numeric_limits<long double>::max(), "\nPlease, make sure you have inputted a non-negative balance value or a valid numeric value..\n");
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

