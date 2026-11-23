#pragma once

#include "IScreen.h"
#include "RichString.h"
#include "InputValidate.h"
#include "Currency.h"
#include <iostream>

using namespace std;
class CurrencyExchangeScreenBase : public IScreen
{
protected:
	CurrencyExchangeScreenBase() {}

public:

	void PrintCurrencyInfo(const Currency& cur)
	{
		const int indentWidth = 40;
		const int labelWidth = 18;
		const std::string indent(indentWidth, ' ');

		std::cout << "\n" << indent << UI_LINE_BOUNDS << "\n";
		std::cout << indent << std::setw(labelWidth + 12) << "Currency Card Details" << "\n";
		std::cout << indent << UI_LINE_BOUNDS << "\n";

		std::cout << indent << std::left << std::setw(labelWidth) << "Country"
			<< ": " << RichString::StartWordsWithUppercase(cur.GetCountry(), " ") << "\n";

		std::cout << indent << std::left << std::setw(labelWidth) << "Currency Code"
			<< ": " << RichString::ToUpper(cur.GetCurrencyCode()) << "\n";

		std::cout << indent << std::left << std::setw(labelWidth) << "Currency Name"
			<< ": " << cur.GetCurrencyName() << "\n";

		std::cout << indent << std::left << std::setw(labelWidth) << "Rate per USD"
			<< ": " << std::fixed << std::setprecision(4) << cur.GetRatePerUSD() << "\n";

		std::cout << indent << UI_LINE_BOUNDS << "\n\n";
	}

	// Wrappers that delegate to the shared input engine and apply field-specific casing.

	string ReadCurrencyCode()
	{
		return _ReadInput();
	}

	string ReadCountryName()
	{
		return _ReadInput();
	}

private:
	string _ReadInput()
	{
		return RichString::Trim(RichString::ToLower(InputValidate::ReadString()));
	}
};

