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

	static Client getEmptyClient()
	{
		return Client(Mode::Empty, "", "", "", "", "", 0.0);
	}
};

