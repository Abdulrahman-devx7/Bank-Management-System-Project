#pragma once
#include "ClientScreenBase.h"
#include "ClientDataManager.h"
#include "InputValidate.h"

class DeleteClientScreen : public ClientScreenBase
{
private:
	ClientDataManager& _manager;
	User& _activeUser;


	void _printClientDeletionStatus(ClientDataManager::DeleteOrUpdateResult result)
	{
		switch (result)
		{
		case ClientDataManager::DeleteOrUpdateResult::Success:
			cout << "\nClient deleted successfully!\n";
			break;

		case ClientDataManager::DeleteOrUpdateResult::Non_updateMode:
			cout << "\nAn error occurred!\nPlease, ensure the client you're trying to add is in a state for update or that you've picked the option to delete a client";
			break;

		case ClientDataManager::DeleteOrUpdateResult::UserNoExist:
			cout << "\nAn error occurred!\nThe client you're trying delete doesn't exist.\nPlease, input unique data.";
			break;

		default:
			cout << "\nUNDEFINED STATE\n";
			break;
		}
	}


	void _deleteClient(const string &accountNumber)
	{
		Client* foundClient = nullptr;

		if ((foundClient = (_manager.Find(accountNumber))) != nullptr)
		{
			PrintInfoCard(*foundClient);

			if (toupper(InputValidate::DetermineAgain("\n\nAre you sure you want to delete this client (Y/N)?\n")) == 'Y')
				_printClientDeletionStatus(_manager.Delete(*foundClient));
		}
		else
			cout << "\n" << GetIndentPadding() << "Client with account number (" << accountNumber << ") is not found!\n";

	}

public:
	DeleteClientScreen(ClientDataManager& injectedManager, User& injectedUser)
		: _manager(injectedManager), _activeUser(injectedUser) {}

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::DeleteClient))
		{
			ResetScreen();
			IScreen::PrintScreenHeader("DELETE CLIENT SCREEN");

			cout << GetIndentPadding() << "Please, enter the account number: ";
			string accountNumber = ReadAccountNumber();
			_deleteClient(accountNumber);
		}

	}
};

