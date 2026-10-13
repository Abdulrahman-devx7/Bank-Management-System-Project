#pragma once
#include "UserDataManager.h"
#include "ClientScreenBase.h"
#include "DepositScreen.h"
#include "WithdrawalScreen.h"
#include "ClientBalancesScreen.h"
#include <unordered_map>

class TransactionMenu : public ClientScreenBase
{
private:
	enum class enMenuChoice : short
	{
        Deposit=1,
        Withdraw=2, 
        TotalBalances=3, 
        Logout=4
	};

    User& _activeUser;
    ClientDataManager& _clientManager;

    std::unordered_map<enMenuChoice, std::unique_ptr<IScreen>> _screens;

    void _PrintMenuUI()
    {
        PrintScreenHeader("TRANSACTIONS SCREEN", "Menu");

        cout << setw(80) << left << "" << right << setw(10) << "[1]" << " Deposit" << "\n";
        cout << setw(80) << left << "" << right << setw(10) << "[2]" << " Withdraw" << "\n";
        cout << setw(80) << left << "" << right << setw(10) << "[3]" << " Total Balances" << "\n";
        cout << setw(80) << left << "" << right << setw(10) << "[4]" << " Return to Main Menu" << "\n";

        cout << setw(80) << left << "" << UI_LINE_BOUNDS << "\n";
    }

    bool IsChoiceExist(enMenuChoice choice)
    {
        return _screens.find(choice) != _screens.end();
    }

public:
    TransactionMenu(User &activeUser, ClientDataManager &clientManager) :
        _activeUser(activeUser), _clientManager(clientManager)
    {
        _screens[enMenuChoice::Deposit] = std::make_unique<DepositScreen>(_clientManager, _activeUser);
        _screens[enMenuChoice::Withdraw] = std::make_unique<WithdrawalScreen>(_clientManager, _activeUser);
        _screens[enMenuChoice::TotalBalances] = std::make_unique<TotalBalancesScreen>(_clientManager, _activeUser);
    }

    void Execute() override
    {
        enMenuChoice menuChoice = enMenuChoice::Logout;
        do
        {
            ResetScreen();
            _PrintMenuUI();
            menuChoice = static_cast<enMenuChoice>(InputValidate::ReadIntegerBetween(1, 4));

            if (IsChoiceExist(menuChoice))
                _screens[menuChoice]->Execute();
            else
                menuChoice = enMenuChoice::Logout;

        } while (menuChoice != enMenuChoice::Logout);
    }
};

