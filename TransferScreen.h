#pragma once
#include "ClientScreenBase.h"
#include "ClientDataManager.h"

class TransferScreen : ClientScreenBase
{
private:

	ClientDataManager& _manager;
	User& _activeUser;

	float _readTransferAmount()
	{
		return InputValidate::ReadFloatBetween(0, FLT_MAX, "\nPlease, enter a valid positive numeric input to withdraw!\n");
	}

	Account::TransferStatus _verifyTransfer(const float transferAmount, Client& sender, Client &receiver)
	{
		if (toupper(InputValidate::DetermineAgain("\nAre you sure you want to perform this transaction (Y/N)? ")) == 'Y')
		{
			return sender.account().Transfer(receiver.account(), transferAmount);
		}
	}


	void _transferUIProcess(string &senderAccountNumber, string &receiverAccountNumber)
	{
		cout << "Please, enter the account number to transfer from: ";
		string accountNumber = ReadAccountNumber();

	}

public:

	TransferScreen(ClientDataManager& injectedManager, User& injectedUser)
		:_manager(injectedManager), _activeUser(injectedUser) {}


	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::Transactions))
		{
			ResetScreen();
			IScreen::PrintScreenHeader("MONEY TRANSFER SCREEN");
		}

	}

};

