#pragma once
#include <iostream>
#include "CurrencyExchangeScreenBase.h"
#include "CurrencyExchangeManager.h"
#include "User.h"

class UpdateCurrencyRateScreen : public CurrencyExchangeScreenBase
{
private:
    CurrencyExchangeManager& _manager;
    User& _activeUser;

    float _readUpdatedRate()
    {
        cout << "\nPlease, enter the new update rate you'd like to input\n";
        return InputValidate::ReadFloatBetween(0, FLT_MAX, "\nNumber is not within a valid range. Please, enter a positive number");
    }

    void _PrintUpdateRateStatus(CurrencyExchangeManager::UpdateResult result)
    {
        switch (result)
        {
        case CurrencyExchangeManager::UpdateResult::Success:
            cout << "\nCurrency rate updated successfully!\n";
            break;

        case CurrencyExchangeManager::UpdateResult::Non_updateMode:
            cout << "\nAn error occurred!\nPlease, ensure the currency you're trying to update is in a state for update or that you've picked the option to update a currency rate.";
            break;

        case CurrencyExchangeManager::UpdateResult::CurrencyNoExist:
            cout << "\nAn error occurred!\nThe currency you're trying to update doesn't exist.\nPlease, enter valid data.";
            break; 

        default:
            cout << "\nUNDEFINED STATE\n";
            break;
        }
    }

    void _UpdateCurrencyUIProcess(const string& currencyCode)
    {
        Currency* targetCurrency = nullptr;

        if ((targetCurrency = (_manager.FindByCurrencyCode(currencyCode))) != nullptr)
        {
            PrintCurrencyInfo(*targetCurrency);

            if (toupper(InputValidate::DetermineAgain("\n\nDo you want to update this currency's data (Y/N)?\n")) == 'Y')
            {
                float newRate = _readUpdatedRate();

                targetCurrency->SetRatePerUSD(newRate);
                _PrintUpdateRateStatus(_manager.Update(*targetCurrency));
                PrintCurrencyInfo(*targetCurrency);
            }
        }
        else
            cout << "\nThe currency with the provided currency code (" << currencyCode << ") is not found!\n";
    }

public:
    UpdateCurrencyRateScreen(CurrencyExchangeManager& manager, User& activeUser)
        : _manager(manager), _activeUser(activeUser) {}

    void Execute() override
    {
        ResetScreen();
        PrintScreenHeader("UPDATE CURRENCY RATE SCREEN");

        cout << "Please, enter the currency code: ";
        _UpdateCurrencyUIProcess(ReadCurrencyCode());
    }
};
