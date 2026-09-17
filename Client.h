#pragma once
#include "IPerson.h"
#include "Account.h"
#include <string>


class Client : public IPerson
{
private:
	Account _clientAccount;
	Mode _mode;
	bool isMarkedForDeletion = false;

public:
	Client(Mode mode, string fullName, string email, string phoneNumber, string accountNumber, string PIN, double balance)
		: IPerson(fullName, email, phoneNumber), _clientAccount(accountNumber, PIN, balance), _mode(mode) {}

	Account &account()
	{
		return _clientAccount;
	}

	Mode getClientStatus()
	{
		return _mode;
	}

	static Client getEmptyClient()
	{
		return Client(Mode::Empty, "", "", "", "", "", 0.0);
	}

	bool IsEmpty()
	{
		return _mode == Mode::Empty;
	}
};

