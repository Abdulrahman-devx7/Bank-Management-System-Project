#pragma once
#include <unordered_map>
#include <memory>
#include <iostream>
#include <iomanip>
#include "IScreen.h"
#include "User.h"
#include "Currency.h"
#include "InputValidate.h"

class CurrencyExchangeMenu : public IScreen
{
private:
	enum class enMenuChoice : short
	{
		ListCurrencies = 1,
		FindCurrency = 2, 
		UpdateCurrency = 3,
		CurrencyCalculator = 4,
		MainMenu = 5
	};

	User& _activeUser;
	unordered_map<string, Currency>& _currencyMapByCode;
	unordered_map<string, Currency*>& _currencyMapByCountry;

	std::unordered_map<enMenuChoice, std::unique_ptr<IScreen>> _screens;

	void _PrintMenuUI()
	{
		PrintScreenHeader("CURRENCY EXCHANGE SCREEN", "Menu");

		cout << setw(80) << left << "" << right << setw(10) << "[1]" << " List Currencies" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[2]" << " Find Currency" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[3]" << " Update Rate" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[4]" << " Currency Calculator" << "\n";
		cout << setw(80) << left << "" << right << setw(10) << "[5]" << " Main Menu" << "\n";

		cout << setw(80) << left << "" << UI_LINE_BOUNDS << "\n";
	}

	bool IsChoiceExist(enMenuChoice choice)
	{
		return _screens.find(choice) != _screens.end();
	}

public:
	CurrencyExchangeMenu(User& activeUser, unordered_map<string, Currency>& currencyMapByCode, unordered_map<string, Currency*>& currencyMapByCountry) :
		_activeUser(activeUser), _currencyMapByCode(currencyMapByCode), _currencyMapByCountry(currencyMapByCountry)
	{
		// Screens for menu options will be initialized here later
	}

	void Execute() override
	{
		enMenuChoice menuChoice = enMenuChoice::MainMenu;
		do
		{
			ResetScreen();
			_PrintMenuUI();
			menuChoice = static_cast<enMenuChoice>(InputValidate::ReadIntegerBetween(1, 5));

			if (IsChoiceExist(menuChoice))
			{
				_screens[menuChoice]->Execute();
				PromptUser("Press any button to go back...");
			}
			else
				menuChoice = enMenuChoice::MainMenu;

		} while (menuChoice != enMenuChoice::MainMenu);
	}
};

