#pragma once
#include "IScreen.h"
#include "UserDataManager.h"
#include "ClientDataManager.h"
#include "ClientListScreen.h"
#include "AddClientScreen.h"
#include "DeleteClientScreen.h"
#include "UpdateClientScreen.h"

class MainMenuScreen : IScreen
{
private:
	User _activeUser;
	ClientDataManager _clientManager;
	UserDataManager _userManager;

public:
	MainMenuScreen(User activeUser, ClientDataManager clientManager, UserDataManager userManager) :
		_activeUser(activeUser), _clientManager(clientManager), _userManager(userManager)
	{

	}
	
	void Execute() override
	{

	}

};

