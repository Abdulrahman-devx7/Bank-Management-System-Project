#pragma once
#include <string>
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
	const string AccountNumber() const
	{
		return _accountNumber;
	}

	const string PIN() const
	{
		return _PIN;
	}

	const double Balance() const
	{
		return _balance;
	}

	void SetPIN(const string& pin)
	{
		_PIN = pin;
	}

	void SetBalance(double amount)
	{
		_balance = amount;
	}

	Account(string accountNumber, string PIN, double balance) 
		: _accountNumber(accountNumber), _PIN(PIN), _balance(balance) {}

	bool Withdraw(double amount)
	{
		if (amount > _balance)
		{
			_balance -= amount;
			return true;
		}
		else return false;
	}

	void Deposit(double amount)
	{
		_balance += amount;
	}
};

