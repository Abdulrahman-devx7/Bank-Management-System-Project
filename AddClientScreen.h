#pragma once
#include "ClientScreenBase.h"
#include "ClientDataManager.h"
#include "InputValidate.h"
#include "BatchSearchStruct.h"
#include <limits>

class AddClientScreen : protected ClientScreenBase
{
private:
	ClientDataManager _manager;

	void _handleBatchSearchError(const SearchFilterOptions options)
	{
		if(options.hasAnyMatch())
		{
			cout << "\nAn error occurred! The following field/s are pre-existent in our database:\n\n";
			if (options.matchAccountNumber)
				cout << "- The Account Number\n";

			if (options.matchEmail)
				cout << "- The Email\n";

			if (options.matchPhoneNumber)
				cout << "- The Phone Number";

			cout << "\n\nPlease, renter these credentials: \n\n";
		}
	}

	void _printNewClientSavingStatus(ClientDataManager::enSaveResult result)
	{
		switch (result)
		{
		case ClientDataManager::enSaveResult::Success:
			cout << "\nClient added successfully!\n";
			break;

		case ClientDataManager::enSaveResult::Non_AddNewMode:
			cout << "\nAn error occurred!\nPlease, ensure the client you're trying to add is in a state for new addition or that you've picked the option to add a new client";
			break;

		case ClientDataManager::enSaveResult::UserExists:
			cout << "\nAn error occurred!\nThe client you're trying to add already exists.\nPlease, input unique data.";
			break;

		default:
			cout << "\nUNDEFINED STATE\n";
			break;
		}
	}

	void _readClientData(Client& client)
	{
		SearchFilterOptions result;
		do
		{
			_handleBatchSearchError(result);

			cout << "Enter the account number: ";
			client.account().SetAccountNumber(ReadAccountNumber());

			cout << "\nEnter the phone number: ";
			client.SetPhoneNumber(ReadPhoneNumber());

			cout << "\nEnter the E-mail: ";
			client.SetEmail(ReadEmail());

			result = _manager.BatchSearch(client.account().AccountNumber(), client.Email(), client.PhoneNumber());

		} while (result.hasAnyMatch());

		cout << "\nEnter the full name: ";
		client.SetFullName(ReadFullName());

		cout << "\nEnter the PIN: ";
		client.account().SetPIN(ReadPIN());

		cout << "Enter the balance";
		client.account().SetBalance(ReadBalance());

		client.SetMode(Client::Mode::New);
	}

public:
	AddClientScreen(const ClientDataManager& injectedManager)
		:_manager(injectedManager) {}

	void Execute() override
	{
		IScreen::PrintScreenHeader("ADD NEW CLIENT SCREEN");

		do
		{
			Client newClient = Client::getNewClientObject();
			_readClientData(newClient);

			_printNewClientSavingStatus(_manager.Add(newClient));

		} while (tolower(InputValidate::DetermineAgain("\nWould you like to add more clients?\n")));
	}
};

