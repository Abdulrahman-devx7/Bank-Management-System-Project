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

public:
    CurrencyCalculatorScreen(CurrencyExchangeManager& manager, User& activeUser)
        : _manager(manager), _activeUser(activeUser) {}

    void Execute() override
    {
        ResetScreen();
        PrintScreenHeader("CURRENCY CALCULATOR SCREEN");
        std::cout << "\n\t\t\t\t\tCurrency Calculator functionality will be implemented here.\n";
    }
};
