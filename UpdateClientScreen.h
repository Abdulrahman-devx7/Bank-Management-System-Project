#pragma once 
#include "ClientScreenBase.h"
#include "ClientDataManager.h"
#include "InputValidate.h"

class UpdateClientScreen : protected ClientScreenBase
{
private:
	ClientDataManager _manager;
	User _activeUser;

	void _PrintNewClientUpdateStatus(ClientDataManager::DeleteOrUpdateResult result)
	{
		switch (result)
		{
		case ClientDataManager::DeleteOrUpdateResult::Success:
			cout << "\nClient updated successfully!\n";
			break;

		case ClientDataManager::DeleteOrUpdateResult::Non_updateMode:
			cout << "\nAn error occurred!\nPlease, ensure the client you're trying to update is in a state for update or that you've picked the option to update a client";
			break;

		case ClientDataManager::DeleteOrUpdateResult::UserNoExist:
			cout << "\nAn error occurred!\nThe client you're trying update doesn't exist.\nPlease, enter valid data.";
			break;

		default:
			cout << "\nUNDEFINED STATE\n";
			break;
		}
	}

	void _ReadClientDataUpdates(Client &targetClient)
	{
		ClientBatchSearch result;
		do
		{
			if (toupper(InputValidate::DetermineAgain("\nDo you want to update the email (Y/N)?\n")) == 'Y')
			{
				cout << "Enter email: ";
				targetClient.SetEmail(ReadEmail());
			}

			if (toupper(InputValidate::DetermineAgain("\nDo you want to update the phone number (Y/N)?\n")) == 'Y')
			{
				cout << "Enter phone number: ";
				targetClient.SetPhoneNumber(ReadPhoneNumber());
			}

			result = _manager.BatchSearch(targetClient.account().AccountNumber(), targetClient.Email(), targetClient.PhoneNumber());
			result.matchAccountNumber = false;

		} while (result.hasAnyMatch());


		if (toupper(InputValidate::DetermineAgain("\nDo you want to update the full name (Y/N)?\n")) == 'Y')
		{
			cout << "Enter full name: ";
			targetClient.SetFullName(ReadFullName());
		}

		if (toupper(InputValidate::DetermineAgain("\nDo you want to update the PIN code (Y/N)?\n")) == 'Y')
		{
			cout << "Enter PIN code: ";
			targetClient.account().SetPIN(ReadPIN());
		}

		if (toupper(InputValidate::DetermineAgain("\nDo you want to update the account balance (Y/N)?\n")) == 'Y')
		{
			cout << "Enter account balance: ";
			targetClient.account().SetBalance(ReadBalance());
		}
	}


	void _UpdateClient(const string& accountNumber)
	{
		Client* foundClient = nullptr;

		if ((foundClient = (_manager.Find(accountNumber))) != nullptr)
		{
			PrintInfoCard(*foundClient);

			if (toupper(InputValidate::DetermineAgain("\n\nDo you want to update this client's data (Y/N)?\n")) == 'Y')
			{
				_ReadClientDataUpdates(*foundClient);
				_manager.Update(*foundClient);
			}

		}
		else
			cout << "\nClient with account number (" << accountNumber << ") is not found!\n";

	}

public: 

	UpdateClientScreen(const ClientDataManager& injectedManager, const User& injectedUser)
	: _manager(injectedManager), _activeUser(injectedUser) {}

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::UpdateClients))
		{
			IScreen::PrintScreenHeader("UPDATE CLIENT SCREEN");

			cout << "Please, enter the account number";
			string accountNumber = ReadAccountNumber();
			_UpdateClient(accountNumber);
		}
	}
};

