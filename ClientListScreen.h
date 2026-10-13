#pragma once
#include "ClientScreenBase.h"
#include "RichString.h"
#include <unordered_map>
class ClientListScreen : public ClientScreenBase
{
private:
	unordered_map<string, Client>& _list;
	User& _activeUser;

    static void PrintFileInfoHeader(const unordered_map<string, Client>& clients)
    {
        std::string indent(SCREEN_INDENT, ' ');

        std::cout << "\n" << indent
            << std::setw((SCREEN_WIDTH + 24) / 2) << std::right
            << "Client List: (" + std::to_string(clients.size()) + ") Client(s)\n";

        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";

        std::cout << indent << "| " << std::left
            << std::setw(16) << "Account Number" << "| "
            << std::setw(10) << "PIN Code" << "| "
            << std::setw(25) << "Full Name" << "| "
            << std::setw(28) << "Email" << "| "
            << std::setw(14) << "Phone Number" << "| "
            << std::setw(14) << "Balance (USD)" << "|\n";

        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";
    }

    static void PrintIndividualTableInfo(const Client& client)
    {
        const std::string indent(SCREEN_INDENT, ' ');

        std::cout << indent << "| " << std::left
            << std::setw(16) << client.account().AccountNumber() << "| "
            << std::setw(10) << client.account().PIN() << "| "
            << std::setw(25) << client.FullName() << "| "
            << std::setw(28) << client.Email() << "| "
            << std::setw(14) << client.PhoneNumber() << "| "
            << std::setw(14) << std::fixed << std::setprecision(2) << client.account().Balance() << "|\n";
    }

    void PrintTable()
    {
        PrintFileInfoHeader(_list);

        for (const auto &[accountNumber, client] : _list)
        {
            PrintIndividualTableInfo(client);
        }
        std::cout << std::string(SCREEN_INDENT, ' ') << std::string(SCREEN_WIDTH, '-') << "\n";
    }

public:

	ClientListScreen(unordered_map<string, Client>& clientList, User& activeUser)
		: _list(clientList), _activeUser(activeUser) {
	}

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::ListClients))
		{
			IScreen::PrintScreenHeader("CLIENTS LIST");
			PrintTable();
		}
	}

};

