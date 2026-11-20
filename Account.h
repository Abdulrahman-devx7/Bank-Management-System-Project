#pragma once
#include <string>
#include <stdexcept>
#include "InputValidate.h"
#include "Client.h"
using namespace std;

class Account
{
	// An account could potentially be locked or deleted so Enums for FSM might be handy here, but for now, we'll reserve that for the client:
	// The data line in a file for instance here would be packing the client both the client data and account data in one line consecutively
	// I wonder how such case is resolved with databases, how are clients and account connected together?
	// Something similar could be seen in DBs, probably

private:
	string _accountNumber;
	string _PIN;
	double _balance = 0.0;

public:

	enum class TransferStatus {Normal, SameAccount, InsufficientBalance, Cancelled};

	bool operator==(const Account& other) const
	{
		return this->AccountNumber() == other.AccountNumber();
	}

	const string& AccountNumber() const
	{
		return _accountNumber;
	}

	const string& PIN() const
	{
		return _PIN;
	}

	double Balance() const
	{
		return _balance;
	}

	void SetPIN(const std::string& pin)
	{
		if (!InputValidate::IsValidPIN(pin))
		{
			throw std::invalid_argument("SetPIN failed: PIN must conform to the required format.");
		}
		_PIN = pin;
	}

	void SetBalance(double transferredAmount)
	{
		if (transferredAmount < 0.0)
		{
			throw std::invalid_argument("SetBalance failed: Account balance cannot be negative.");
		}
		_balance = transferredAmount;
	}

	void SetAccountNumber(const std::string& accountNumber)
	{
		if (!InputValidate::IsValidAccountNumber(accountNumber))
		{
			throw std::invalid_argument("SetAccountNumber failed: Invalid account number format.");
		}
		_accountNumber = accountNumber;
	}

	Account(const string& accountNumber, const string& PIN, double balance) 
		: _accountNumber(accountNumber), _PIN(PIN), _balance(balance) {}

	bool Withdraw(double transferredAmount)
	{
		if (transferredAmount < _balance)
		{
			_balance -= transferredAmount;
			return true;
		}
		else return false;
	}

	void Deposit(double transferredAmount)
	{
		_balance += transferredAmount;
	}

	TransferStatus Transfer(Account& receiver, double transferredAmount)
	{
		if (receiver == *this)
			return TransferStatus::SameAccount;

		if (transferredAmount > _balance)
			return TransferStatus::InsufficientBalance;

		this->Withdraw(transferredAmount);
		receiver.Deposit(transferredAmount);

		return TransferStatus::Normal;
	}

};

