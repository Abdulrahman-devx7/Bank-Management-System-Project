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

public:
    UpdateCurrencyRateScreen(CurrencyExchangeManager& manager, User& activeUser)
        : _manager(manager), _activeUser(activeUser) {}

    void Execute() override
    {
        ResetScreen();
        PrintScreenHeader("UPDATE CURRENCY RATE SCREEN");
        std::cout << "\n\t\t\t\t\tUpdate Currency Rate functionality will be implemented here.\n";
    }
};
