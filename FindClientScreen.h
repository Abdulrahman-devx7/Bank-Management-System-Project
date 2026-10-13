#pragma once
#include "ClientScreenBase.h"
#include "ClientDataManager.h"
class FindClientScreen : public ClientScreenBase
{
private:
	ClientDataManager& _manager;
	User& _activeUser;

	void _FindUserUIProcess(const string &accountNumber)
	{
		Client* foundUser = _manager.Find(accountNumber);

		if (foundUser != nullptr)
			PrintInfoCard(*foundUser);
		else
			cout << "\n" << GetIndentPadding() << "Client with account number [" << accountNumber << "] is not found!\n";
	}

public:
	FindClientScreen(ClientDataManager& injectedManager, User& injectedUser)
		: _manager(injectedManager), _activeUser(injectedUser) {
	}

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::FindClient))
		{
			ResetScreen();
			IScreen::PrintScreenHeader("FIND CLIENT SCREEN");

			cout << GetIndentPadding() << "Please, enter the account number: ";
			string accountNumber = ReadAccountNumber();

			_FindUserUIProcess(accountNumber);
		}
	}

};

