#pragma once
#include <iostream>
#include "CurrencyExchangeScreenBase.h"
#include "CurrencyExchangeManager.h"
#include "User.h"

class CurrencyCalculatorScreen : public CurrencyExchangeScreenBase
{
private:
    CurrencyExchangeManager& _manager;
    User& _activeUser;

    struct ExchangeData
    {
        Currency from = Currency::getEmptyCurrency();
        Currency to = Currency::getEmptyCurrency();
        float exchangeAmount = 0.0;
    };

    Currency _fetchCurrency(const string &currencyCode)
    {
        Currency* target = _manager.FindByCurrencyCode(currencyCode);

        if (target != nullptr)
            return *target;

        return Currency::getEmptyCurrency();
    }

    void _showConversionResult(const ExchangeData& data)
    {
        cout << "\nConverting from :\n\n";
        PrintCurrencyInfo(data.from);

        cout << "\nTo: \n\n";
        PrintCurrencyInfo(data.to);

        cout << "\n" << data.exchangeAmount << " " << data.from.GetCurrencyCode() << " = "
            << (1.0/data.from.GetRatePerUSD()) * data.exchangeAmount * data.to.GetRatePerUSD() << data.to.GetCurrencyCode();
    }

    bool _isCurrencyExist(const Currency &currency)
    {
        if (currency.GetMode() == Currency::enMode::Empty)
        {
            cout << "\nThis currency doesn't exist, please, enter a valid currency\n";
            return false;
        }
        else
            return true;
    }

    void _currencyCalculatorUIProcess()
    {
        ExchangeData data;

        cout << "\nPlease, enter currency code for the currency you need to transfer from: ";
        string fromCode = ReadCurrencyCode();

        data.from = _fetchCurrency(fromCode);

        if (!_isCurrencyExist(data.from))
            return;

        cout << "\nPlease, enter currency code for 2nd currency you need to transfer to: ";
        string toCode = ReadCurrencyCode();

        data.to = _fetchCurrency(toCode);

        if (!_isCurrencyExist(data.to))
            return;

        cout << "\nPlease, enter the amount you'd like to exchange: ";
        cin >> data.exchangeAmount;

        _showConversionResult(data);
    }

public:
    CurrencyCalculatorScreen(CurrencyExchangeManager& manager, User& activeUser)
        : _manager(manager), _activeUser(activeUser) {}

    void Execute() override
    {
        ResetScreen();
        PrintScreenHeader("CURRENCY CALCULATOR SCREEN");
        _currencyCalculatorUIProcess();

    }
};
