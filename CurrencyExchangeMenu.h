#pragma once
#include "IScreen.h"
#include "User.h"

class CurrencyExchangeMenu : IScreen
{
private:
	enum class enMenuChoice : short
	{
		ListCurrencies=1,
		FindCalculator=2, 
		UpdateCurrency=3,
		CurrencyCalculator=4,
		MainMenu=5
	};

	User& _activeUser;
	// currency manager
	// HashMap that uses country codes as keys and currency objects as values
	// Anohter HashMap that uses the country name as the code, but doesn't actually hold the data, 
	// but pointers of the currency object, all of which point to the currency objecct nodes in the
	// first mentioned HashMap. This is going to be handeled carefully in the data access layer

	// The rest of the class should be implemented very similarly to TransactionMenu. We'll get into that
	// after finishing some missing stuff.

};

