#pragma once
#include "ClientScreenBase.h"
#include "ClientDataManager.h"

class DepositScreen : public ClientScreenBase
{
private:
	ClientDataManager& _manager;
	User& _activeUser;

	float _ReadDepositNumber()
	{
		return InputValidate::ReadFloatBetween(0, FLT_MAX, "\nPlease, enter a valid positive numeric input to withdraw!\n");
	}

	bool _VerifyDeposit(const long double depositAmount, Client &client)
	{
		if (toupper(InputValidate::DetermineAgain("\nAre you sure you want to perform this transaction (Y/N)? ")) == 'Y')
		{
			client.account().Deposit(depositAmount);
			return true;
		}
		else 
			return false;
	}

	void _DepositUIProcess(const string &accountNumber)
	{
		Client* foundClient = _manager.Find(accountNumber);

		if (foundClient != nullptr)
		{
			cout << "\nThe following are the client data: \n\n";
			PrintInfoCard(*foundClient);
			long double depositAmount = _ReadDepositNumber();

			if (_VerifyDeposit(depositAmount, *foundClient))
				cout << "\n\nDone! Your deposit of " << depositAmount << " has been added successfully. Your new balance is: "
				<< foundClient->account().Balance() << " USD\n\n";
			else
				cout << "\n\nTransaction has been canceled.\n\n";
		}
	}

public:

	DepositScreen(ClientDataManager& injectedManager, User& injectedUser)
		:_manager(injectedManager), _activeUser(injectedUser) {
	}

	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::Transactions))
		{
			ResetScreen();
			IScreen::PrintScreenHeader("DEPOSIT SCREEN");

			cout << GetIndentPadding() << "Please, enter the account number: ";
			string accountNumber = ReadAccountNumber();
			_DepositUIProcess(accountNumber);
		}
	}


};

