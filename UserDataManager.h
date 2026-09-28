#pragma once
#include <unordered_map>
#include "UserDataAccess.h"

class UserDataManager
{
public:
	enum class enSaveResult
	{ 
		Success = 1,
		Non_AddNewMode = 2,
		UserExists = 3 };
	enum class DeleteOrUpdateResult 
	{
		Success = 1,
		Non_updateMode = 2,
		UserNoExist = 3 
	};
	enum class enAuthStatus
	{
		Normal = 1,
		Success = 2,
		InvalidCredentials = 3,
	};

private:
	// Instantiating the data manager object with these two ensures that we if we were to change source we're
	// saving to or loading from, we'd merely have to pass the file name we want via the constructor of the data manager object 
	// and be able to use this inside the object. Thus, completely avoiding any hardcoding here in this class
	unordered_map<string, User> _userMap;
	string _sourceFile;

public:

	// Don't forget passing by reference for the getter of the same map in the session controller.
	UserDataManager(unordered_map<string, User>& injectedMap, string sourceFileName)
		:_userMap(injectedMap), _sourceFile(sourceFileName) {}

	enSaveResult Add(User& target)
	{
		if (target.Status() == User::Mode::New)
		{
			_userMap.insert({ target.Username(), target });
			UserDataAccess::AppendUserRecord(target, "users.txt");
			target.SetMode(User::Mode::Update);

			return enSaveResult::Success;
		}
		else if (_userMap.find(target.Username()) != _userMap.end())
			return enSaveResult::UserExists;

		else
			return enSaveResult::Non_AddNewMode;

	}

	User* Find(const User& target)
	{
		auto userIt = _userMap.find(target.Username());

		if (userIt != _userMap.end())
			return &(userIt->second);

		return nullptr;
	}

	User* Find(const string& username, const string& password)
	{
		auto userIt = _userMap.find(username);

		if (userIt != _userMap.end())
		{
			if(userIt->second.Password()==password)
				return &(userIt->second);
		}

		return nullptr;
	}

	// don't forget the username validation that prohibits spaces in the middle of it. this exists 
	// in the add user screen of the procedural version but doesn't in update user
	// SO REMEMBER ADDING THAT TO THE UI LAYER.
	DeleteOrUpdateResult Update(const User &previousData, const User &newData)
	{
		User* targetPointer = Find(previousData);
		if (targetPointer == nullptr)
			return DeleteOrUpdateResult::UserNoExist;

		switch (previousData.Status())
		{
		case User::Mode::Update:
			_userMap.erase(targetPointer->Username());
			_userMap.insert_or_assign(newData.Username(), newData);
			UserDataAccess::SaveData(_sourceFile, _userMap);
			return DeleteOrUpdateResult::Success;

		default:
			return DeleteOrUpdateResult::Non_updateMode;
		}
	}

	DeleteOrUpdateResult Delete(User& target)
	{
		User* targetPointer = Find(target);
		if (targetPointer == nullptr)
			return DeleteOrUpdateResult::UserNoExist;

		switch (targetPointer->Status())
		{
		case User::Mode::Update:
			targetPointer->MarkForDeletion();
			return DeleteOrUpdateResult::Success;

		default:
			return DeleteOrUpdateResult::Non_updateMode;
		}
	}

	// Should this merely return an object or is the Boolean a sufficient indicator? We have the find method returning a pointer, so...
	enAuthStatus Authenticate(const string &username, const string &password)
	{
		auto targetUserIt = _userMap.find(username);

		if ((targetUserIt == _userMap.end()) || (targetUserIt->second.Password() != password))
			return enAuthStatus::InvalidCredentials;
		else
			return enAuthStatus::Success;
	}

	void AddBaseUser(const string& username, const string& password)
	{
		User baseUser = User::getEmptyUser();

		baseUser.setUsername(username);
		baseUser.setPassword(password);
		baseUser.SetMode(User::Mode::Update);

		_userMap.insert({ username, baseUser });
	}


};

