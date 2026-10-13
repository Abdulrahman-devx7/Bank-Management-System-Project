#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include "ClientScreenBase.h"
#include "ClientDataManager.h"
#include "User.h"

class TotalBalancesScreen : public ClientScreenBase
{
private:
    ClientDataManager& _manager;
    User& _activeUser;

    inline static const int BALANCES_TABLE_WIDTH = 77;
    inline static const int BALANCES_INDENT = SCREEN_INDENT + ((SCREEN_WIDTH - BALANCES_TABLE_WIDTH) / 2);

    static void PrintClientBalancesHeader(size_t clientCount)
    {
        const std::string indent(BALANCES_INDENT, ' ');

        std::cout << "\n" << indent
            << std::setw((BALANCES_TABLE_WIDTH + 28) / 2) << std::right
            << "Balances List: (" + std::to_string(clientCount) + ") Client(s)\n";

        std::cout << indent << std::string(BALANCES_TABLE_WIDTH, '-') << "\n";

        std::cout << indent << "| " << std::left
            << std::setw(16) << "Account Number" << "| "
            << std::setw(40) << "Full Name" << "| "
            << std::setw(14) << "Balance (USD)" << "|\n";

        std::cout << indent << std::string(BALANCES_TABLE_WIDTH, '-') << "\n";
    }

    static void PrintIndividualClientBalanceRow(const Client& client)
    {
        const std::string indent(BALANCES_INDENT, ' ');

        std::cout << indent << "| " << std::left
            << std::setw(16) << client.account().AccountNumber() << "| "
            << std::setw(40) << client.FullName() << "| "
            << std::setw(14) << std::fixed << std::setprecision(2) << client.account().Balance() << "|\n";
    }

    void PrintBalancesTable()
    {
        const auto& clientMap = _manager.GetClientMap();
        double totalBalances = 0.0;

        PrintClientBalancesHeader(clientMap.size());

        if (clientMap.empty())
        {
            const std::string indent(BALANCES_INDENT, ' ');
            std::cout << indent << "| " << std::left
                << std::setw(BALANCES_TABLE_WIDTH - 4) << "No clients available in the system." << " |\n";
        }
        else
        {
            for (const auto& [accountNumber, client] : clientMap)
            {
                PrintIndividualClientBalanceRow(client);
                totalBalances += client.account().Balance();
            }
        }

        const std::string indent(BALANCES_INDENT, ' ');
        std::cout << indent << std::string(BALANCES_TABLE_WIDTH, '-') << "\n";

        std::cout << indent << std::setw(BALANCES_TABLE_WIDTH - 15) << std::right << "Total Balances = $"
            << std::left << std::fixed << std::setprecision(2) << totalBalances << "\n\n";
    }

public:
    TotalBalancesScreen(ClientDataManager& injectedManager, User& injectedUser)
        : _manager(injectedManager), _activeUser(injectedUser) {
    }

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::Transactions))
        {
            PrintScreenHeader("TOTAL BALANCES SCREEN");
            PrintBalancesTable();
        }
    }
};