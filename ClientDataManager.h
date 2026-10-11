#pragma once
#include "ClientDataAccess.h"
#include "ClientBatchSearch.h"

class ClientDataManager
{
public:
	enum class enSaveResult { Success = 1, Non_AddNewMode = 2, UserExists = 3 };
	enum class DeleteOrUpdateResult { Success = 1, Non_updateMode = 2, UserNoExist = 3 };

private:
	unordered_map<string, Client> _clientMap;
	string _sourceFile;

public:

	ClientDataManager(unordered_map<string, Client>& injectedMap, string sourceFileName)
		:_clientMap(injectedMap), _sourceFile(sourceFileName) {
	}

	enSaveResult Add(Client &target)
	{
		if (target.Status() == Client::Mode::New)
		{
			_clientMap.insert({ target.account().AccountNumber(), target});
			ClientDataAccess::AppendUserRecord(target, _sourceFile);
			target.SetMode(Client::Mode::Update);

			return enSaveResult::Success;
		}
		else if (_clientMap.find(target.account().AccountNumber()) != _clientMap.end())
			return enSaveResult::UserExists;

		else
			return enSaveResult::Non_AddNewMode;

	}

	Client* Find(const Client& target)
	{
		auto clientIt = _clientMap.find(target.account().AccountNumber());

		if (clientIt != _clientMap.end())
		{
			clientIt->second.SetMode(Client::Mode::Update);
			return &(clientIt->second);
		}

		return nullptr;
	}

	Client* Find(const string& accountNumber)
	{
		auto clientIt = _clientMap.find(accountNumber);

		if (clientIt != _clientMap.end())
		{
			clientIt->second.SetMode(Client::Mode::Update);
			return &(clientIt->second);
		}

		return nullptr;
	}

	DeleteOrUpdateResult Update(const Client& newData)
	{
		Client* targetPointer = Find(newData.account().AccountNumber());
		if (targetPointer == nullptr)
			return DeleteOrUpdateResult::UserNoExist;

		switch (newData.Status())
		{
		case Client::Mode::Update:
			_clientMap.erase(targetPointer->account().AccountNumber());
			_clientMap.insert_or_assign(newData.account().AccountNumber(), newData);
			ClientDataAccess::SaveData(_sourceFile, _clientMap);
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
			ClientDataAccess::SaveData(_sourceFile, _clientMap);
			return DeleteOrUpdateResult::Success;

		default:
			return DeleteOrUpdateResult::Non_updateMode;
		}
	}

	unordered_map<string, Client>& GetClientMap()
	{
		return _clientMap;
	}

	ClientBatchSearch BatchSearch(const string &accountNumber, const string &email, const string &phoneNumber)
	{
		ClientBatchSearch options;
		for (const auto& [acc_number, client] : _clientMap)
		{
			if (client.account().AccountNumber() == accountNumber)
				options.matchAccountNumber = true;

			if (client.Email() == email)
				options.matchEmail = true;

			if (client.PhoneNumber() == phoneNumber)
				options.matchPhoneNumber = true;

			if (options.hasAnyMatch())
				return options;
		}
	}

};

