#pragma once
#include "ClientDataAccess.h"
#include "UserDataAccess.h"
#include "IScreen.h"

class Session
{
public:
	enum class enLoginStatus { LoggedIn = 1, LoggedOut = 2 };

private:
	unordered_map<string, Client> _clientMap;
	unordered_map<string, User> _userMap;

	User _activeUser = User::getEmptyUser();
	ClientDataAccess::enDataStatus _clientFileStatus
		= ClientDataAccess::enDataStatus::eFileNotFound;

	UserDataAccess::enDataStatus _userFileStatus
		= UserDataAccess::enDataStatus::eFileNotFound;

	enLoginStatus _status = enLoginStatus::LoggedOut;

	void _AddBaseUser(const string &username, const string &password)
	{
		// This logic is a place holder until we create the user business logic class and then use it here.
		User baseUser = User::getEmptyUser();

		baseUser.setUsername(username);
		baseUser.setPassword(password);
		baseUser.SetMode(User::Mode::Update);

		GetUserMap().insert({ username, baseUser });
	}

	void _HandleUsersFileNotFound()
	{
		IScreen::PrintScreenHeader("ERROR OCCURRED", "The user database connected to\nthe system isn't found!\nPlease, contact your admin to resolve\n the issue");
	}

	void _HandleEmptyUsersFile()
	{
		IScreen::PrintScreenHeader("ERROR OCCURRED", "The database connected to\nthe system doesn't\ncontain any users.\nUse these credentials to start using\nthe system:\nUsername: admin1 --- Pass : system111");
		IScreen::PromptUser("Press any button to proceed..\n");
		_AddBaseUser("admin1", "system111");
	}

public:

	Session()
	{
		_clientFileStatus = ClientDataAccess::LoadData("clients.txt", GetClientMap(), "#//#");
		_userFileStatus = UserDataAccess::LoadData("users.txt", GetUserMap(), "#//#");
	}

	unordered_map<string, Client>& GetClientMap() 
	{
		return _clientMap;
	}

	unordered_map<string, User>& GetUserMap() 
	{
		return _userMap;
	}

	ClientDataAccess::enDataStatus getClientFileStatus() const
	{
		return _clientFileStatus;
	}

	UserDataAccess::enDataStatus getUserFileStatus() const 
	{
		return _userFileStatus;
	}

	enLoginStatus GetLoginStatus() const
	{
		return _status;
	}

	void SetLoginStatus(enLoginStatus status)
	{
		_status = status;
	}

	void Run()
	{
		if (getUserFileStatus() != UserDataAccess::enDataStatus::eFileNotFound)
		{
			// handle the case where a user file is empty and then proceed with login screen
			if (getUserFileStatus() == UserDataAccess::enDataStatus::eFileEmpty)
				_HandleEmptyUsersFile();

			// afterwards, check if something is wrong with the client file or not as well before proceeding with login

			// login screen with authentication there, which is followed by routing to the main menu with the two loops once everything succeeds.
		}
		else
		{
			_HandleUsersFileNotFound();
			return;
		}
	}
};

