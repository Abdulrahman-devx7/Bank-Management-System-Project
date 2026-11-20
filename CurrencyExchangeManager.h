#pragma once

#include "Currrency.h"
#include "CurrencyExchangeDataAccess.h"
#include <unordered_map>

class CurrencyExchangeManager
{
public:
	enum class UpdateResult
	{
		Success = 1,
		Non_updateMode = 2,
		CurrencyNoExist = 3
	};

private:

	string _sourceFile;
	unordered_map<string, Currency>& _currencyCodeMap;
	unordered_map<string, Currency*>& _currencyNameMap;

	void UpdateBothMaps(const Currency& newData)
	{
		auto [it, inserted] = _currencyCodeMap.insert_or_assign(newData.GetCurrencyCode(),newData);
		_currencyNameMap[newData.GetCountry()] = &(it->second);
	}
public:
	CurrencyExchangeManager(const string& fileName, unordered_map<string, Currency>& currencyCodeMap, unordered_map<string, Currency*>& CurrencyNameMap)
		:_sourceFile(fileName), _currencyCodeMap(currencyCodeMap), _currencyNameMap(CurrencyNameMap)
	{}

	const string& getSourceFileName() const
	{
		return _sourceFile;
	}

	Currency* Find(const Currency& target)
	{
		return FindByCurrencyCode(target.GetCurrencyCode());
	}

	Currency* FindByCurrencyCode(const string &currencyCode)
	{
		auto currencyIt = _currencyCodeMap.find(currencyCode);

		if (currencyIt != _currencyCodeMap.end())
		{
			currencyIt->second.SetMode(Currency::enMode::Update);
			return &(currencyIt->second);
		}

		return nullptr;
	}

	Currency* FindByCurrencyName(const string &currencyCountryName)
	{
		auto currencyIt = _currencyNameMap.find(currencyCountryName);
		
		if (currencyIt != _currencyNameMap.end())
		{
			currencyIt->second->SetMode(Currency::enMode::Update);
			return currencyIt->second;
		}

		return nullptr;
	}

	UpdateResult Update(const Currency& newData)
	{
		Currency* targetPointer = FindByCurrencyCode(newData.GetCurrencyCode());
		if (targetPointer == nullptr)
			return UpdateResult::CurrencyNoExist;

		switch (newData.GetMode())
		{
		case Currency::enMode::Update:
		{
			UpdateBothMaps(newData);
			CurrencyExchangeDataAccess::SaveData(_sourceFile, _currencyCodeMap);
			return UpdateResult::Success;
		}

		default:
			return UpdateResult::Non_updateMode;
		}
	}

	const unordered_map<string, Currency>& GetCurrencyCodeMap() const 
	{
		return _currencyCodeMap;
	}

	const unordered_map<string, Currency*>& GetCurrencyNameMap() const 
	{
		return _currencyNameMap;
	}

};
