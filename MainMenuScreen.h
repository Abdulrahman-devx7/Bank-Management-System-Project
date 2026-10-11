#pragma once
#include <unordered_map>
#include <memory>
#include "IScreen.h"
#include "UserDataManager.h"
#include "ClientDataManager.h"
#include "ClientListScreen.h"
#include "AddClientScreen.h"
#include "DeleteClientScreen.h"
#include "UpdateClientScreen.h"
#include "FindClientScreen.h"

class MainMenuScreen : IScreen
{
private:
	enum class enMainMenuChoice : short
	{
		ClientList = 1,
		AddClient = 2,
		DeleteClient = 3,
		UpdateClient = 4,
		FindClient = 5,
		Transactions = 6,
		ManageUsers = 7,
		Logout = 8
	};

	User _activeUser;
	ClientDataManager _clientManager;
	UserDataManager _userManager;

	std::unordered_map<enMainMenuChoice, std::unique_ptr<IScreen>> _screens;

	void _PrintMainMenuUI()
	{
		PrintScreenHeader("MAIN MENU");

		cout << setw(80) << left << "" << right << setw(10) << "[1]" << " Show Client List" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[2]" << " Add New Client" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[3]" << " Delete Client" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[4]" << " Update Client Info" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[5]" << " Find Client" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[6]" << " Transactions" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[7]" << " Manage Users" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[8]" << " Logout" << "\n";

		cout << setw(80) << left << "" << UI_LINE_BOUNDS << "\n";
	}

	bool IsChoiceExist(enMainMenuChoice choice)
	{
		return _screens.find(choice) != _screens.end();
	}

public:
	MainMenuScreen(User activeUser, ClientDataManager clientManager, UserDataManager userManager) :
		_activeUser(activeUser), _clientManager(clientManager), _userManager(userManager)
	{
		_screens[enMainMenuChoice::ClientList] = std::make_unique<ClientListScreen>(_clientManager.GetClientMap());
		_screens[enMainMenuChoice::AddClient] = std::make_unique<AddClientScreen>(_clientManager, _activeUser);
		_screens[enMainMenuChoice::DeleteClient] = std::make_unique<DeleteClientScreen>(_clientManager, _activeUser);
		_screens[enMainMenuChoice::UpdateClient] = std::make_unique<UpdateClientScreen>(_clientManager, _activeUser);
		_screens[enMainMenuChoice::FindClient] = std::make_unique<FindClientScreen>(_clientManager, _activeUser);
	}
	
	void Execute() override
	{
		enMainMenuChoice MenuChoice = enMainMenuChoice::Logout;
		do
		{
			ResetScreen();
			_PrintMainMenuUI();
			MenuChoice = static_cast<enMainMenuChoice>(InputValidate::ReadIntegerBetween(1, 8));

			if (IsChoiceExist(MenuChoice))
			{
				_screens[MenuChoice]->Execute();
				PromptUser("Press any button to go back");
			}
			else
				MenuChoice = enMainMenuChoice::Logout;

		} while (MenuChoice!=enMainMenuChoice::Logout);
	}

};

