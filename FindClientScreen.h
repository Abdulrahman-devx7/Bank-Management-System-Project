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
			cout << "\nUser with username [" << accountNumber << "] is not found!\n";
	}

public:
	FindClientScreen(ClientDataManager& injectedManager, User& injectedUser)
		: _manager(injectedManager), _activeUser(injectedUser) {
	}

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::FindClient))
		{
			IScreen::PrintScreenHeader("FIND USER SCREEN");

			cout << "\nEnter the username: ";
			string username = ReadAccountNumber();

			_FindUserUIProcess(username);
		}
	}

};

