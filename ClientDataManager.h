#pragma once
#include "ClientDataAccess.h"
class ClientDataManager
{
public:
	enum class enSaveResult { Success = 1, Non_AddNewMode = 2, UserExists = 3 };
	enum class DeleteOrUpdateResult { Success = 1, Non_updateMode = 2, UserNoExist = 3 };

private:
	unordered_map<string, Client> _userMap;
	string _sourceFile;

public:

	ClientDataManager(unordered_map<string, Client>& injectedMap, string sourceFileName)
		:_userMap(injectedMap), _sourceFile(sourceFileName) {
	}

	enSaveResult Add(Client &target)
	{
		if (target.Status() == Client::Mode::New)
		{
			_userMap.insert({ target.account().AccountNumber(), target});
			ClientDataAccess::AppendUserRecord(target, "users.txt");
			target.SetMode(Client::Mode::Update);

			return enSaveResult::Success;
		}
		else if (_userMap.find(target.account().AccountNumber()) != _userMap.end())
			return enSaveResult::UserExists;

		else
			return enSaveResult::Non_AddNewMode;

	}

	Client* Find(const Client& target)
	{
		auto userIt = _userMap.find(target.account().AccountNumber());

		if (userIt != _userMap.end())
			return &(userIt->second);

		return nullptr;
	}

	DeleteOrUpdateResult Update(const Client& previousData, const Client& newData)
	{
		Client* targetPointer = Find(previousData);
		if (targetPointer == nullptr)
			return DeleteOrUpdateResult::UserNoExist;

		switch (previousData.Status())
		{
		case Client::Mode::Update:
			_userMap.erase(targetPointer->account().AccountNumber());
			_userMap.insert_or_assign(newData.account().AccountNumber(), newData);
			ClientDataAccess::SaveData(_sourceFile, _userMap);
			return DeleteOrUpdateResult::Success;

		default:
			return DeleteOrUpdateResult::Non_updateMode;
		}
	}

	DeleteOrUpdateResult Delete(Client& target)
	{
		Client* targetPointer = Find(target);
		if (targetPointer == nullptr)
			return DeleteOrUpdateResult::UserNoExist;

		switch (targetPointer->Status())
		{
		case Client::Mode::Update:
			targetPointer->MarkForDeletion();
			return DeleteOrUpdateResult::Success;

		default:
			return DeleteOrUpdateResult::Non_updateMode;
		}
	}



};

