#pragma once
#include <unordered_map>
#include <memory>
#include "IScreen.h"
#include "UserDataManager.h"
#include "ClientDataManager.h"
#include "DepositScreen.h"
#include "WithdrawalScreen.h"
#include "ClientBalancesScreen.h"
#include "TransferScreen.h"
#include "MoneyTransferScreen.h"
#include "TransferRecordStruct.h"
#include <iostream>

class TransactionMenu : public IScreen
{
private:
	enum class enMenuChoice : short
	{
		Deposit=1,
		Withdraw=2, 
		TotalBalances=3, 
		Transfer=4,
		TransferRegistry=5,
		MainMenu=6
	};

    User& _activeUser;
    ClientDataManager& _clientManager;
    vector<TransferRecordStruct>& _transferRegisters;

    std::unordered_map<enMenuChoice, std::unique_ptr<IScreen>> _screens;

    void _PrintMenuUI()
    {
        PrintScreenHeader("TRANSACTIONS SCREEN", "Menu");

        cout << setw(80) << left << "" << right << setw(10) << "[1]" << " Deposit" << "\n";
        cout << setw(80) << left << "" << right << setw(10) << "[2]" << " Withdraw" << "\n";
        cout << setw(80) << left << "" << right << setw(10) << "[3]" << " Total Balances" << "\n";
        cout << setw(80) << left << "" << right << setw(10) << "[4]" << " Transfer Screen" << "\n";
        cout << setw(80) << left << "" << right << setw(10) << "[5]" << " Transfer Registry" << "\n";
        cout << setw(80) << left << "" << right << setw(10) << "[6]" << " Return to Main Menu" << "\n";

        cout << setw(80) << left << "" << UI_LINE_BOUNDS << "\n";
    }

    bool IsChoiceExist(enMenuChoice choice)
    {
        return _screens.find(choice) != _screens.end();
    }

public:
    TransactionMenu(User &activeUser, ClientDataManager &clientManager, vector<TransferRecordStruct>& transferRegisters) :
        _activeUser(activeUser), _clientManager(clientManager), _transferRegisters(transferRegisters)
    {
        _screens[enMenuChoice::Deposit] = std::make_unique<DepositScreen>(_clientManager, _activeUser);
        _screens[enMenuChoice::Withdraw] = std::make_unique<WithdrawalScreen>(_clientManager, _activeUser);
        _screens[enMenuChoice::TotalBalances] = std::make_unique<TotalBalancesScreen>(_clientManager, _activeUser);
        _screens[enMenuChoice::Transfer] = std::make_unique<TransferScreen>(_clientManager, _activeUser);
        _screens[enMenuChoice::TransferRegistry] = std::make_unique<MoneyTransferScreen>(_transferRegisters, _activeUser);

    }

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::Transactions))
        {
            enMenuChoice menuChoice = enMenuChoice::MainMenu;
            do
            {
                ResetScreen();
                _PrintMenuUI();
                menuChoice = static_cast<enMenuChoice>(InputValidate::ReadIntegerBetween(1, 6));

                if (IsChoiceExist(menuChoice))
                {
                    _screens[menuChoice]->Execute();
                    PromptUser("Press any button to go back...");
                }
                else
                    menuChoice = enMenuChoice::MainMenu;

            } while (menuChoice != enMenuChoice::MainMenu);
        }
    }
};

