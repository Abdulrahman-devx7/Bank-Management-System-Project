#pragma once
#include <iostream>
#include "CurrencyExchangeScreenBase.h"
#include "CurrencyExchangeManager.h"
#include "User.h"

class FindCurrencyScreen : public CurrencyExchangeScreenBase
{
private:
    CurrencyExchangeManager& _manager;
    User& _activeUser;

public:
    FindCurrencyScreen(CurrencyExchangeManager& manager, User& activeUser)
        : _manager(manager), _activeUser(activeUser) {}

    void Execute() override
    {
        ResetScreen();
        PrintScreenHeader("FIND CURRENCY SCREEN");
        std::cout << "\n\t\t\t\t\tFind Currency functionality will be implemented here.\n";
    }
};
