#pragma once
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "InputValidate.h"

class FindUserScreen : protected UserScreenBase
{
private:
	UserDataManager _manager;
	User _activeUser;

	void _FindUser(const string& username)
	{
		User targetUser = User::getEmptyUser();
		targetUser.setUsername(username);

		User* foundUser = _manager.Find(targetUser);

		if (foundUser != nullptr)
		{
			cout << "\n";
			PrintInfoCard(*foundUser);
		}
		else
		{
			cout << "\nUser with username [" << username << "] is not found!\n";
		}
	}

public:
	FindUserScreen(const UserDataManager& injectedManager, const User& injectedUser)
		: _manager(injectedManager), _activeUser(injectedUser) {}

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
		{
			IScreen::PrintScreenHeader("FIND USER SCREEN");

			cout << "\nEnter the username: ";
			string username = ReadUsername();

			_FindUser(username);
		}
	}
};

