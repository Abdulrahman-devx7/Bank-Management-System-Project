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
	enum Permissions : int32_t {
		None = 0, All = -1, ListClients = 1, AddNewClient = 2, DeleteClient = 4, UpdateClients = 8,
		FindClient = 16, Transactions = 32, ManageUsers = 64
	};

	User(Mode mode, string fullName, string email, string phoneNumber, string username, string password, int32_t permissions)
		: IPerson(fullName, email, phoneNumber),
		_username(username), _password(password), _permissions(permissions), _mode(mode) {}

	bool CheckAccessPermission(Permissions Permission)
	{
		return (Permission & this->_permissions) == Permission;
	}

	static User getEmptyUser()
	{
		return User(Mode::Empty, "", "", "", "", "", 0);
	}

	string Username()
	{
		return _username;
	}

	string Password()
	{
		return _password;
	}

	int32_t getPermissions()
	{
		return _permissions;
	}

	void setUsername(string username)
	{
		_username = username;
	}

	void setPassword(string password)
	{
		_password = password;
	}
	
	void setPermissions(int32_t permissions)
	{
		_permissions = permissions;
	}
};
