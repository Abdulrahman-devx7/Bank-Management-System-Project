#pragma once
#include "ClientDataAccess.h"
#include "UserDataAccess.h"
#include "UserDataManager.h"
#include "IScreen.h"

class Session
{

private:
	enum class enLoginStatus { LoggedIn = 1, LoggedOut = 2 };
	enum class SystemReadiness { Ready = 1, EssentialSystemFilesProblematic = 2, NoBootUp = 3 };

	const string client_file_name = "clients.txt";
	const string users_file_name = "users.txt";

	// We are thinking of delegating these containers to the managers later on after finishing the program files.
	unordered_map<string, Client> _clientMap;
	unordered_map<string, User> _userMap;

	UserDataManager _userManager{ _userMap, "users.txt" };
	// Client data manager 

	User _activeUser = User::getEmptyUser();
	ClientDataAccess::enDataStatus _clientFileStatus
		= ClientDataAccess::enDataStatus::eFileNotFound;

	UserDataAccess::enDataStatus _userFileStatus
		= UserDataAccess::enDataStatus::eFileNotFound;

	enLoginStatus _status = enLoginStatus::LoggedOut;

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

public:

	UserDataManager& GetUserManager() { return _userManager; }

	// Should add these two to the entity managers instead

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

	// Read-only getter for system readiness
	SystemReadiness GetSystemReadiness() const
	{
		return _systemState;
	}

	void Boot()
	{
		_clientFileStatus = ClientDataAccess::LoadData(client_file_name, GetClientMap(), "#//#");
		_userFileStatus = UserDataAccess::LoadData(users_file_name, GetUserMap(), "#//#");

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

		}
		// login screen with authentication there, which is followed by routing to the main menu with the two loops once everything succeeds.
		// 
	}
};

