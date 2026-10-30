#pragma once
#include "IPerson.h"
#include <string>
#include <cstdint>

class User : public IPerson
{
public:
	enum class Mode { Empty = 1, Update = 2, New = 3 };

private:
	string _username;
	string _password;
	int32_t _permissions;
	Mode _mode;
	bool _isMarkedForDeletion = false;

public: 
	enum Permissions : int32_t {
		None = 0, All = -1, ListClients = 1, AddNewClient = 2, DeleteClient = 4, UpdateClients = 8,
		FindClient = 16, Transactions = 32, ManageUsers = 64, LoginRegisteryLog = 128
	};

	User(Mode mode, const string& fullName, const string& email, const string& phoneNumber, const string& username, const string& password, int32_t permissions)
		: IPerson(fullName, email, phoneNumber),
		_mode(mode), _username(username), _password(password), _permissions(permissions) {}

	bool CheckAccessPermission(Permissions Permission) const
	{
		return (Permission & this->_permissions) == Permission;
	}

	static User getEmptyUser()
	{
		return User(Mode::Empty, "", "", "", "", "", 0);
	}

	const string& Username() const
	{
		return _username;
	}

	const string& Password() const
	{
		return _password;
	}

	int32_t getPermissions() const
	{
		return _permissions;
	}

	Mode Status() const
	{
		return _mode;
	}

	bool IsEmpty() const
	{
		return _mode == Mode::Empty;
	}

	bool IsMarkedForDeletion() const
	{
		return _isMarkedForDeletion;
	}

	void setUsername(const string& username)
	{
		_username = username;
	}

	void setPassword(const string& password)
	{
		_password = password;
	}
	
	void setPermissions(int32_t permissions)
	{
		_permissions = permissions;
	}

	void SetMode(Mode mode)
	{
		_mode = mode;
	}

	void MarkForDeletion()
	{
		_isMarkedForDeletion = true;
	}

};
