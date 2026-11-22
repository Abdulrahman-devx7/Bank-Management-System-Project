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
			<< ": " << cur.GetCountry() << "\n";

		std::cout << indent << std::left << std::setw(labelWidth) << "Currency Code"
			<< ": " << cur.GetCurrencyCode() << "\n";

		std::cout << indent << std::left << std::setw(labelWidth) << "Currency Name"
			<< ": " << cur.GetCurrencyName() << "\n";

		std::cout << indent << std::left << std::setw(labelWidth) << "Rate per USD"
			<< ": " << std::fixed << std::setprecision(4) << cur.GetRatePerUSD() << "\n";

		std::cout << indent << UI_LINE_BOUNDS << "\n\n";
	}

	string ReadCurrencyCode()
	{
		string input;
		getline(std::cin, input);

		RichString::Trim(input);
		RichString::ToLower(input);
		return input;
	}

	string ReadCountryName()
	{
		string input;
		getline(std::cin, input);

		RichString::Trim(input);
		RichString::ToLower(input);
		return input;
	}
};

