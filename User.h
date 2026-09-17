#pragma once
#include "IPerson.h"
#include <string>
#include <cstdint>

class User : public IPerson
{
private:
	string _username;
	string _password;
	int32_t _permissions;
	Mode _mode;
	bool isMarkedForDeletion = false;

public: 
	User(Mode mode, string fullName, string email, string phoneNumber, string username, string password, int32_t permissions)
		: IPerson(fullName, email, phoneNumber),
		_username(username), _password(password), _permissions(permissions), _mode(mode) {}
};


