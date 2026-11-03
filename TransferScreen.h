#pragma once
#include "ClientScreenBase.h"
#include "ClientDataManager.h"

class TransferScreen : public ClientScreenBase
{
private:

	ClientDataManager& _manager;
	User& _activeUser;

	float _readTransferAmount()
	{
		return InputValidate::ReadFloatBetween(0, FLT_MAX, "\nPlease, enter a valid positive numeric input to withdraw!\n");
	}

	void _showTransferResult(Account::TransferStatus status)
	{
		switch (status)
		{
		case Account::TransferStatus::Normal:
		{
			ClientDataAccess::SaveData(_manager.getSourceFileName(), _manager.GetClientMap());
			std::cout << "\nTransfer completed successfully.\n";
			break;
		}
		case Account::TransferStatus::SameAccount:
			std::cout << "\nError: Target account is identical to source account.\n";
			break;
		case Account::TransferStatus::InsufficientBalance:
			std::cout << "\nError: Amount exceeds available balance.\n";
			break;
		case Account::TransferStatus::Cancelled:
			std::cout << "\nTransfer Cancelled.\n";
			break;
		}
	}

	Account::TransferStatus _verifyTransfer(const float transferAmount, Client& sender, Client &receiver)
	{
		if (toupper(InputValidate::DetermineAgain("\nAre you sure you want to perform this transaction (Y/N)? ")) == 'Y')
			return sender.account().Transfer(receiver.account(), transferAmount);
		else
			return Account::TransferStatus::Cancelled;
	}

	bool _handleAccountExistence(const Client *target)
	{
		if (target != nullptr)
			PrintInfoCard(*target);
		else
		{
			cout << "\nThis account doesn't exist in our database!\n";
			return false;
		}

		return true;
	}

	void _transferUIProcess()
	{
		cout << "Please, enter the account number to transfer from: ";
		string senderAccountNumber = ReadAccountNumber();

		Client* sender = _manager.Find(senderAccountNumber);
		if (!_handleAccountExistence(sender))
			return;


		cout << "Please, enter the account number to transfer to: ";
		string receiverAccountNumber = ReadAccountNumber();

		Client* receiver = _manager.Find(receiverAccountNumber);
		if (!_handleAccountExistence(receiver))
			return;

		cout << "\nPlease, enter the transfer amount: ";
		float transferAmount = _readTransferAmount();

		_showTransferResult(_verifyTransfer(transferAmount, *sender, *receiver));
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
			_transferUIProcess();
		}
	}
};

