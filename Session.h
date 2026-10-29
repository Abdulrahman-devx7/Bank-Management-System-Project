#pragma once
#include "ClientDataAccess.h"
#include "UserDataAccess.h"
#include "UserDataManager.h"
#include "ClientDataManager.h"
#include "LoginScreen.h"
#include "MainMenuScreen.h"

class Session
{
private:
	enum class SystemReadiness { Ready = 1, EssentialSystemFilesProblematic = 2, NoBootUp = 3 };
	short _numTries = 3;

	const string client_file_name = "clients.txt";
	const string users_file_name = "users.txt";

	// We are thinking of delegating these containers to the managers later on after finishing the program files.
	unordered_map<string, Client> _clientMap;
	unordered_map<string, User> _userMap;

	UserDataManager _userManager{ _userMap, users_file_name };
	ClientDataManager _clientManager{ _clientMap, client_file_name };

	User _activeUser = User::getEmptyUser();

	ClientDataAccess::enDataStatus _clientFileStatus
		= ClientDataAccess::enDataStatus::eFileNotFound;

	UserDataAccess::enDataStatus _userFileStatus
		= UserDataAccess::enDataStatus::eFileNotFound;

	SystemReadiness _systemState = SystemReadiness::NoBootUp;

	// POSITION OF THIS FUNCTION IS TEMPORARY UNTIL IT COULD GET DELEGATED TO THE BUSINESS LOGIC LAYER FOR USERS
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

	UserDataManager& GetUserManager() { return _userManager; }

	ClientDataManager& GetClientManager() { return _clientManager; }

	ClientDataAccess::enDataStatus getClientFileStatus() const
	{
		return _clientFileStatus;
	}

	UserDataAccess::enDataStatus getUserFileStatus() const 
	{
		return _userFileStatus;
	}

	// Read-only getter for system readiness
	SystemReadiness GetSystemReadiness() const
	{
		return _systemState;
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
		_clientFileStatus = ClientDataAccess::LoadData(client_file_name, _clientMap, "#//#");
		_userFileStatus = UserDataAccess::LoadData(users_file_name, _userMap, "#//#");

		if (getUserFileStatus() != UserDataAccess::enDataStatus::eFileNotFound)
		{
			if (getUserFileStatus() == UserDataAccess::enDataStatus::eFileEmpty)
				_HandleEmptyUsersFile();

			if (getClientFileStatus() == ClientDataAccess::enDataStatus::eFileNotFound)
				_HandleClientFileNotFound();
		}
		else
			_HandleUsersFileNotFound();

		if (getUserFileStatus() != UserDataAccess::enDataStatus::eFileNotFound
			&& getClientFileStatus() != ClientDataAccess::enDataStatus::eFileNotFound)

			_systemState = SystemReadiness::Ready;
		else
			_systemState = SystemReadiness::EssentialSystemFilesProblematic;
	}

	void Run()
	{
        if (GetSystemReadiness() == SystemReadiness::NoBootUp)
			_HandleRunWithoutBoot();

		else if (GetSystemReadiness() == SystemReadiness::Ready)
		{
			// Instantiate a login screen object with the session dep. injection
			// With the login screen object having a dependency injection, it could access the 
			// integer indicating the number of false credentials entering. This means that 
			// our fellow login screen object would be changing an important indicator here.
			// Thus take an action from here and onward when extending the system. 
			// The auth would be done here normally followed by instantiating the main menu class with the dep. injection
			// This is going to be encircled in while(true) loop to keep the program running.
			// The login screen object would be connected to the session controller and potential checkers
			// could be added here for extensibility inside this while(true) loop
			LoginScreen login(_userManager, _activeUser, _numTries);
			MainMenuScreen startupScreen(_activeUser, _clientManager, _userManager);
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
