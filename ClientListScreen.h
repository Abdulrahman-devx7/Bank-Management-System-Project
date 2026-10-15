#pragma once
#include "ClientScreenBase.h"
#include "RichString.h"
#include <unordered_map>
class ClientListScreen : public ClientScreenBase
{
private:
	unordered_map<string, Client>& _list;
	User& _activeUser;

    inline static const int CLIENT_TABLE_WIDTH = 127;

    static void PrintFileInfoHeader(const unordered_map<string, Client>& clients)
    {
        int indent = GetCenterIndent(CLIENT_TABLE_WIDTH);
        std::string pad(indent, ' ');

        std::cout << "\n" << pad
            << CenterText("Client List: (" + std::to_string(clients.size()) + ") Client(s)", CLIENT_TABLE_WIDTH) << "\n";

        std::cout << pad << std::string(CLIENT_TABLE_WIDTH, '-') << "\n";

        std::cout << pad << "| " << std::left
            << std::setw(15) << "Account Number" << "| "
            << std::setw(10) << "PIN Code" << "| "
            << std::setw(32) << "Full Name" << "| "
            << std::setw(30) << "Email" << "| "
            << std::setw(14) << "Phone Number" << "| "
            << std::setw(15) << "Balance (USD)" << "|\n";

        std::cout << pad << std::string(CLIENT_TABLE_WIDTH, '-') << "\n";
    }

    static void PrintIndividualTableInfo(const Client& client)
    {
        int indent = GetCenterIndent(CLIENT_TABLE_WIDTH);
        const std::string pad(indent, ' ');

        std::cout << pad << "| " << std::left
            << std::setw(15) << FormatCell(client.account().AccountNumber(), 15) << "| "
            << std::setw(10) << FormatCell(client.account().PIN(), 10) << "| "
            << std::setw(32) << FormatCell(client.FullName(), 32) << "| "
            << std::setw(30) << FormatCell(client.Email(), 30) << "| "
            << std::setw(14) << FormatCell(client.PhoneNumber(), 14) << "| "
            << std::setw(15) << std::fixed << std::setprecision(2) << client.account().Balance() << "|\n";
    }

    void PrintTable()
    {
        PrintFileInfoHeader(_list);

        if (_list.empty())
        {
            int indent = GetCenterIndent(CLIENT_TABLE_WIDTH);
            std::cout << std::string(indent, ' ') << "| " << std::left
                << std::setw(CLIENT_TABLE_WIDTH - 4) << "No clients available in the system." << " |\n";
        }
        else
        {
            for (const auto &[accountNumber, client] : _list)
            {
                PrintIndividualTableInfo(client);
            }
        }
        int indent = GetCenterIndent(CLIENT_TABLE_WIDTH);
        std::cout << std::string(indent, ' ') << std::string(CLIENT_TABLE_WIDTH, '-') << "\n";
    }

public:

	ClientListScreen(unordered_map<string, Client>& clientList, User& activeUser)
		: _list(clientList), _activeUser(activeUser) {
	}

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::ListClients))
		{
            ResetScreen();
			IScreen::PrintScreenHeader("CLIENTS LIST");
			PrintTable();
		}
	}

};

