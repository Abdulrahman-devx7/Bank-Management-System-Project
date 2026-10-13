#pragma once
#include <iostream>
#include <climits>
#include <utility>
#include <cmath>
#include <regex>
#include "RichString.h"
using namespace std;

class InputValidate
{
private:
    static bool IsInputMalformed()
    {
        return cin.fail() || (std::cin.peek() != '\n' && std::cin.peek() != EOF);
    }

public:

    template <typename T>
    static bool IsNumberBetween(T Number, T From, T To)
    {
        return (Number >= From && Number <= To);
    }

    static long long ReadInteger(const string& errorMessage = "Invalid Input! Please, enter a valid input\n")
    {
        long long Number = 0;
        cin >> Number;

        while (IsInputMalformed())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            cout << errorMessage;
            cin >> Number;
        }
        return Number;
    }

    static long long ReadIntegerBetween(long long from, long long to, const string& errorMessage = "Number is not within range, enter a valid number\n")
    {
        long long Number = ReadInteger(errorMessage);

        while (!IsNumberBetween(Number, from, to))
        {
            cout << errorMessage;
            Number = ReadInteger();
        }
        return Number;
    }

    static long double ReadFloat(const string& errorMessage = "Invalid Input! Please, enter a valid input\n")
    {
        long double Number = 0;
        cin >> Number;

        while (IsInputMalformed())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            cout << errorMessage;
            cin >> Number;
        }
        return Number;
    }

    static long double ReadFloatBetween(long double from, long double to, const string& errorMessage = "Number is not within range, enter a valid number\n")
    {
        long double Number = ReadFloat(errorMessage);

        while (!IsNumberBetween(Number, from, to))
        {
            cout << errorMessage;
            Number = ReadFloat();
        }
        return Number;
    }

    static string ReadString()
    {
        string text;
        getline(cin >> ws, text);
        return text;
    }

    static bool IsValidAccountNumber(const string& accountNumber)
    {
        static const regex pattern("[a-zA-Z]{3}[0-9]{4}");
        return regex_match(accountNumber, pattern);
    }

    static bool IsValidPIN(const string& PIN)
    {
        return PIN.length() == 6 &&
            all_of(PIN.begin(), PIN.end(), [](unsigned char c) {
            return isdigit(c);
                });
    }
    static bool IsValidPhoneNumber(const string& phoneNumber)
    {
        static const regex pattern("(010|011|012|015)[0-9]{8}");
        return regex_match(phoneNumber, pattern);
    }

    static string NormalizeUsername(string text)
    {
        RichString::Trim(text);
        RichString::ToLower(text);

        return text;
    }

    static std::string NormalizeAccountNumber(std::string accNumber)
    {
        RichString::Trim(accNumber);
        RichString::ToUpper(accNumber);
        return accNumber;
    }

    static char DetermineAgain(const string& message)
    {
        char PlayAgain;
        cout << message;
        cin >> PlayAgain;

        while (cin.fail() || (toupper(PlayAgain) != 'N' && toupper(PlayAgain) != 'Y')
            || (std::cin.peek() != '\n' && std::cin.peek() != EOF))
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Please, enter a valid Choice (Y/N)!\n";

            cin >> PlayAgain;
        }
        cin.ignore();

        return PlayAgain;
    }

    static bool AreNamesOnlyLetters(const vector<string>& names)
    {
        for (const string& name : names)
        {
            if (!all_of(name.begin(), name.end(), ::isalpha))
                return false;
        }
        return true;
    }

    static bool CheckSpacesInUsername(const string& username)
    {
        for (int i = 0; i < username.length(); i++)
        {
            if (isspace(username[i]))
                return true;
        }
        return false;
    }

};

