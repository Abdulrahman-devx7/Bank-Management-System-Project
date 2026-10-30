#pragma once
#include "ClientDataAccess.h"
#include "UserDataAccess.h"
#include "UserDataManager.h"
#include "ClientDataManager.h"
#include "LoginScreen.h"
#include "MainMenuScreen.h"
#include "LoginDataRegistery.h"
#include "RegisterLine.h"

class Session
{
private:
	enum class SystemReadiness { Ready = 1, EssentialSystemFilesProblematic = 2, NoBootUp = 3 };
	short _numTries = 3;

	const string CLIENT_FILE_NAME = "clients.txt";
	const string USERS_FILE_NAME = "users.txt";
	const string LOGIN_LOG_NAME = "registery_log.txt";

	// We are thinking of delegating these containers to the managers later on after finishing the program files.
	unordered_map<string, Client> _clientMap;
	unordered_map<string, User> _userMap;
	vector<RegisterLine> _LoginRegistry;

	UserDataManager _userManager{ _userMap, USERS_FILE_NAME };
	ClientDataManager _clientManager{ _clientMap, CLIENT_FILE_NAME };

	User _activeUser = User::getEmptyUser();

	ClientDataAccess::enDataStatus _clientFileStatus
		= ClientDataAccess::enDataStatus::eFileNotFound;

	UserDataAccess::enDataStatus _userFileStatus
		= UserDataAccess::enDataStatus::eFileNotFound;

	SystemReadiness _systemState = SystemReadiness::NoBootUp;

	void _HandleUsersFileNotFound()
	{
		IScreen::PrintScreenHeader("ERROR OCCURRED", "The user database connected to\nthe system isn't found!\nPlease, contact your admin to resolve\n the issue");
	}

	void _HandleEmptyUsersFile()
	{
		IScreen::PrintScreenHeader("ERROR OCCURRED", "The database connected to\nthe system doesn't\ncontain any users.\nUse these credentials to start using\nthe system:\nUsername: admin1 --- Pass : system111");
		IScreen::PromptUser("Press any button to proceed..\n");
		_userManager.AddBaseUser("admin1", "system111");
	}

	void _HandleClientFileNotFound()
	{
		IScreen::PrintScreenHeader("ERROR OCCURRED", "The client database connected\nto the system doesn't exist.\nPlease contact your admin.");
		IScreen::PromptUser("Press any button to halt the application.\n");
	}

	void _HandleRunWithoutBoot()
	{
		IScreen::PrintScreenHeader("ERROR OCCURRED", "Please, boot the system's\nessentials before trying to\n run the application.");
		IScreen::PromptUser("Press any button to halt the application.\n");
	}

	void _HandleLockingUser()
	{
		IScreen::ResetScreen();
		cout << "We have locked you out of the system due to security reasons. \n\n";
		IScreen::PromptUser("");
	}

public:

	Session()
	{

	}

	const User& GetActiveUser() const
	{
		return _activeUser;
	}
	
	void Boot()
	{
		_clientFileStatus = ClientDataAccess::LoadData(CLIENT_FILE_NAME, _clientMap, "#//#");
		_userFileStatus = UserDataAccess::LoadData(USERS_FILE_NAME, _userMap, "#//#");
		LoginDataRegistery::LoadLog(LOGIN_LOG_NAME, _LoginRegistry, "#//#");

		if (_userFileStatus != UserDataAccess::enDataStatus::eFileNotFound)
		{
			if (_userFileStatus == UserDataAccess::enDataStatus::eFileEmpty)
				_HandleEmptyUsersFile();

			if (_clientFileStatus == ClientDataAccess::enDataStatus::eFileNotFound)
				_HandleClientFileNotFound();
		}
		else
			_HandleUsersFileNotFound();

		if (_userFileStatus != UserDataAccess::enDataStatus::eFileNotFound
			&& _clientFileStatus != ClientDataAccess::enDataStatus::eFileNotFound)

			_systemState = SystemReadiness::Ready;
		else
			_systemState = SystemReadiness::EssentialSystemFilesProblematic;
	}

	void Run()
	{
        if (_systemState == SystemReadiness::NoBootUp)
			_HandleRunWithoutBoot();

		else if (_systemState == SystemReadiness::Ready)
		{
			LoginScreen login(_userManager, _activeUser, _numTries);
			MainMenuScreen startupScreen(_activeUser, _clientManager, _userManager, _LoginRegistry);
			while (true)
			{
				login.setAuthStatus(UserDataManager::enAuthStatus::Normal);
				login.Execute();

				if (_numTries != 0)
				{
					IScreen::SetActiveUser(_activeUser);
					startupScreen.Execute();
					IScreen::ClearActiveUser();
					_numTries = 3;
				}
				else
				{
					_HandleLockingUser();
					break;
				}
			}

		}
	}
};
