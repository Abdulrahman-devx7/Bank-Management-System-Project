#pragma once
#include <string>

using namespace std;

class Currency
{
public:
	enum class enMode
	{
		Empty = 1,
		Update = 2
	};

private:

	// These properties must remain read-only properties
	string _country;
	string _currencyCode;
	string _currencyName;

	float _ratePerUSD = 0.0;
	enMode _mode;

public:

	Currency(enMode mode, string country, string currencyCode, string currencyName, float rate) :
		_country(country), _currencyCode(currencyCode), _currencyName(currencyName), _ratePerUSD(rate), _mode(mode)
	{
	}

	const string& GetCountry() const
	{
		return _country;
	}

	const string& GetCurrencyCode() const
	{
		return _currencyCode;
	}

	const string& GetCurrencyName() const
	{
		return _currencyName;
	}

	enMode GetMode() const
	{
		return _mode;
	}

	void SetMode(enMode mode)
	{
		_mode = mode;
	}

	float GetRatePerUSD() const
	{
		return _ratePerUSD;
	}

	void SetRatePerUSD(float rate)
	{
		_ratePerUSD = rate;
	}

};

