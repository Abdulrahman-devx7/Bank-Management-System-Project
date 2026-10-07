#pragma once 
#include "ClientScreenBase.h"
#include "ClientDataManager.h"

class FindUserScreen : protected ClientScreenBase
{
private:
	ClientDataManager _manager;
	User _activeUser;

	void _FindUser(const string &accountNumber)
	{
		Client* foundClient = nullptr;
		if ((foundClient = (_manager.Find(accountNumber))) != nullptr)
			PrintInfoCard(*foundClient);
		else
			cout << "\nClient with account number (" << accountNumber << ") is not found!\n";

	}

public:

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::DeleteClient))
		{
			cout << "\nPlease, enter the account number: ";
			string accountNumber = ReadAccountNumber();
			_FindUser(accountNumber);
		}
	}
};

