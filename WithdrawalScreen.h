#pragma once
#include "ClientScreenBase.h"
#include "ClientDataManager.h"

class WithdrawalScreen : protected ClientScreenBase
{
private:
	ClientDataManager _manager;
	User _activeUser;
	Client _foundClient = Client::getEmptyClient();
	//A client member has been added to this class to reduce parameter drilling and maintain accessibility 
	enum class WithdrawalStatus {
		Succeeded = 1,
		Canceled = 2,
		InsufficientBalance = 3
	};

	float _ReadWithdrawNumber()
	{
		return InputValidate::ReadFloatBetween(0, FLT_MAX, "\nPlease, enter a valid positive numeric input to withdraw!\n");
	}

	bool CheckBalanceForWithdrawal(long double withdrawalAmount)
	{
		return (withdrawalAmount > _foundClient.account().Balance());
	}

	void _PrintWithdrawalStatus(const WithdrawalStatus status, long double amount)
	{
		switch (status)
		{
		case WithdrawalStatus::Succeeded:
			cout << "\n\nDone! Your withdrawal of " << amount << " has been done successfully. Your new balance is: "
				<< _foundClient.account().Balance() << " USD\n\n";
			break;

		case WithdrawalStatus::Canceled: 
			cout << "\n\nTransaction has been canceled.";
			break;

		case WithdrawalStatus::InsufficientBalance:
			cout << "\n\nInsufficient balance!\nYou can only withdraw up to " << _foundClient.account().Balance();
			break;
		}
	}

	WithdrawalStatus _VerifyWithdrawal(long double withdrawalAmount)
	{
		if (toupper(InputValidate::DetermineAgain("\nAre you sure you want to perform this transaction (Y/N)? ")) == 'Y')
		{
			if (!CheckBalanceForWithdrawal(withdrawalAmount))
			{
				_foundClient.account().Withdraw(withdrawalAmount);
				return WithdrawalStatus::Succeeded;
			}
			else
				return WithdrawalStatus::InsufficientBalance;
		}
		else
			return WithdrawalStatus::Canceled;

	}

	void _WithdrawalUIProcess(const string& accountNumber)
	{
		Client* foundClient = _manager.Find(accountNumber);

		if (foundClient != nullptr)
		{
			cout << "\nThe following are the client data: \n\n";
			PrintInfoCard(*foundClient);
			long double withdrawAmount = _ReadWithdrawNumber();
			_PrintWithdrawalStatus(_VerifyWithdrawal(withdrawAmount), withdrawAmount);
		}
	}


public:

	WithdrawalScreen(const ClientDataManager& injectedManager, User& injectedUser)
		:_manager(injectedManager), _activeUser(injectedUser) {}


	void Execute() override
	{
		if (CheckAccessRights(_activeUser, User::Permissions::Transactions))
		{
			IScreen::PrintScreenHeader("DEPOSIT SCREEN");

			cout << "Please, enter the account number";
			string accountNumber = ReadAccountNumber();
			_WithdrawalUIProcess(accountNumber);
		}
	}

};

