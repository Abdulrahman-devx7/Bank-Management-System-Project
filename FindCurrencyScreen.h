#pragma once
#include <iostream>
#include "CurrencyExchangeScreenBase.h"
#include "CurrencyExchangeManager.h"
#include "User.h"
#include "InputValidate.h"

class FindCurrencyScreen : public CurrencyExchangeScreenBase
{
private:
    CurrencyExchangeManager& _manager;
    User& _activeUser;

    enum class FindingChoice {CountryName=1, CountryCode=2};

    void _displayResult(const Currency* currency)
    {
        if (currency == nullptr)
            cout << "Currency not found!\nPlease enter a valid currency code/country name";
        else
            PrintCurrencyInfo(*currency);
    }

    FindingChoice _readUserChoice()
    {
        cout << "What would you like to use find the currency?\n[1] Country Name\n[2] Country Code \n";
        return static_cast<FindingChoice> (InputValidate::ReadIntegerBetween(1, 2));
    }

    Currency* _determineFindingMethod(const FindingChoice choice, const string &input)
    {
        switch (choice) 
        {
        case FindingChoice::CountryName:
            return _manager.FindByCurrencyName(input);

        case FindingChoice::CountryCode:
            return _manager.FindByCurrencyCode(input);

        default:
            return nullptr;
        }
    }

    void _FindCurrencyUIProcess()
    {
        FindingChoice choice = _readUserChoice();
        string code;

        switch (choice)
        {
        case FindingChoice::CountryCode:
        {
             cout << "\nEnter the currency code of the country\n";
             code = ReadCurrencyCode();
             break;
        }
        case FindingChoice::CountryName:
        {
            cout << "\nEnter the currency name of the country\n";
            code = ReadCountryName();
            break;
        }
        }

        _displayResult(_determineFindingMethod(choice, code));
    }

public:
    FindCurrencyScreen(CurrencyExchangeManager& manager, User& activeUser)
        : _manager(manager), _activeUser(activeUser) {}

    void Execute() override
    {
        ResetScreen();
        PrintScreenHeader("FIND CURRENCY SCREEN");
        _FindCurrencyUIProcess();
    }
};
