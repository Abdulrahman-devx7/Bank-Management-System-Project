#pragma once
#include "IScreen.h"
#include "UserDataManager.h"

class LoginScreen : public IScreen
{
private:
	struct stLoginCredentials
	{
		string inputUsername;
		string inputPassword;
	};

	string ReadUsername()
	{
		string username = "";

		// Reading only - prompt moved to screen functions to separate concerns
		getline(cin >> ws, username);

		return username;
	}

	string ReadPassword()
	{
		string password = "";
		getline(cin >> ws, password);

		return password;
	}

	UserDataManager::enAuthStatus _authStatus = UserDataManager::enAuthStatus::Normal;
	stLoginCredentials _credentials;

	UserDataManager& _usersManager;
	User& _activeUser;

	void _ScreenHeaderMessage()
	{
		ResetScreen();
		switch (_authStatus)
		{
		case UserDataManager::enAuthStatus::Normal:
			PrintScreenHeader("LOGIN SCREEN");
			break;

		case UserDataManager::enAuthStatus::InvalidCredentials:
			PrintScreenHeader("LOGIN SCREEN", "Invalid username or password!");
			break;

		case UserDataManager::enAuthStatus::Success:
			break;
		default:
			// fallback
			break;
		}
	}

	stLoginCredentials _ReadLoginCredentials()
	{
		stLoginCredentials loginData;
		string pad = GetIndentPadding();

		cout << pad << "Username: ";
		loginData.inputUsername = ReadUsername();

		InputValidate::NormalizeUsername(loginData.inputUsername);
		// 🚨🚨 This will be added later via including a library dedicated for input validation and normalization
		// ConvertToLowerCase(loginData.inputUsername);

		cout << "\n" << pad << "Password: ";
		loginData.inputPassword = ReadPassword();

		return loginData;
	}

public:
	// Could add a counter to lock the user after a certain time of wrong password or invalid entries 
	// No details for now
	// If this were to extend, we could just bundle the necessary state vars into a struct and pass them along
	LoginScreen(UserDataManager &manager, User &activeUser)
		: _usersManager(manager), _activeUser(activeUser) {}

	UserDataManager::enAuthStatus AuthStatus()
	{
		return _authStatus;
	}

	void setAuthStatus(UserDataManager::enAuthStatus status)
	{
		_authStatus = status;
	}

	void Execute() override
	{
		do
		{
			ResetScreen();
			_ScreenHeaderMessage();

			stLoginCredentials loginData = _ReadLoginCredentials();
			_authStatus = _usersManager.Authenticate(loginData.inputUsername, loginData.inputPassword);

			if (_authStatus == UserDataManager::enAuthStatus::Success)
				_activeUser = *_usersManager.Find(loginData.inputUsername, loginData.inputPassword);

		} while (_authStatus != UserDataManager::enAuthStatus::Success);

		PromptUser("Login Succeeded! Please, press any button to enter the main menu");
	}

};

