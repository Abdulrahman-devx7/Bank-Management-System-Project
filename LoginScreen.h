#pragma once
#include "IScreen.h"
#include "UserDataManager.h"

class LoginScreen : public IScreen
{
private:
	short &_numTries;
	struct stLoginCredentials
	{
		string inputUsername;
		string inputPassword;
	};

	UserDataManager::enAuthStatus _authStatus = UserDataManager::enAuthStatus::Normal;
	enum class LockStatus { Locked, NotLocked };
	LockStatus _Locked = LockStatus::NotLocked;

	stLoginCredentials _credentials;

	UserDataManager& _usersManager;
	User& _activeUser;

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

	void _ScreenHeaderMessage()
	{
		ResetScreen();
		switch (_authStatus)
		{
		case UserDataManager::enAuthStatus::Normal:
			PrintScreenHeader("LOGIN SCREEN");
			break;

		case UserDataManager::enAuthStatus::InvalidCredentials:
		{
			string Subtitle = "Invalid username or password! You have " + to_string(_numTries) + " tries left!";
			PrintScreenHeader("LOGIN SCREEN", Subtitle);
		}
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

		cout << "\nUsername: ";
		loginData.inputUsername = ReadUsername();

		InputValidate::NormalizeUsername(loginData.inputUsername);

		cout << "\nPassword: ";
		loginData.inputPassword = ReadPassword();

		return loginData;

	}

	void ManageAuthAction(const stLoginCredentials &loginData)
	{
		if (_authStatus == UserDataManager::enAuthStatus::Success)
			_activeUser = *_usersManager.Find(loginData.inputUsername, loginData.inputPassword);

		else if (_authStatus == UserDataManager::enAuthStatus::InvalidCredentials)
			_numTries--;	

		if (_numTries == 0)
		{
			_Locked = LockStatus::Locked;
			return;
		}

	}

public:
	// Could add a counter to lock the user after a certain time of wrong password or invalid entries 
	// No details for now
	// If this were to extend, we could just bundle the necessary state vars into a struct and pass them along
	LoginScreen(UserDataManager &manager, User &activeUser, short &numTries)
		: _usersManager(manager), _activeUser(activeUser), _numTries(numTries) {}

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

			ManageAuthAction(loginData);
			if (_Locked == LockStatus::Locked)
				return;

		} while (_authStatus != UserDataManager::enAuthStatus::Success);

		_authStatus = UserDataManager::enAuthStatus::Normal;
		PromptUser("Login Succeeded! Please, press any button to enter the main menu");
	}

};

