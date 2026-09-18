#pragma once
#include "IPerson.h"
#include "Account.h"
#include <string>


class Client : public IPerson
{
public:
	enum class Mode { Empty = 1, Update = 2, New = 3 };

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

	Mode Status()
	{
		return _mode;
	}

	bool IsEmpty()
	{
		return _mode == Mode::Empty;
	}

	static Client getEmptyClient()
	{
		return Client(Mode::Empty, "", "", "", "", "", 0.0);
	}

	void SetMode(Mode mode)
	{
		_mode = mode;
	}

};

