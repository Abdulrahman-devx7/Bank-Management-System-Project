#pragma once
#include <unordered_map>
#include "UserDataAccess.h"
class ClientDataManager
{
public:
	enum class enSaveResult { Success = 1, Non_AddNewMode = 2, UserExists = 3 };
	enum class DeleteOrUpdateResult { Success = 1, Non_updateMode = 2, UserNoExist = 3 };
private:
	unordered_map<string, User> _userMap;

public:

	// Don't forget passing by reference for the getter of the same map in the session controller.
	ClientDataManager(unordered_map<string, User>& injectedMap)
		:_userMap(injectedMap) {}

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

	User* Find(User& target)
	{
		auto userIt = _userMap.find(target.Username());

		if (userIt != _userMap.end())
			return &(userIt->second);

		return nullptr;
	}

	// don't forget the username validation that prohibits spaces in the middle of it. this exists 
	// in the add user screen of the procedural version but doesn't in update user
	// SO REMEMBER ADDING THAT TO THE UI LAYER.
	DeleteOrUpdateResult Update(User& target)
	{
		if (target.Status() == User::Mode::Update)
		{

		}
	}

};

