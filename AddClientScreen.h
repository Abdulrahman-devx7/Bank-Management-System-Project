#pragma once
#include "ClientScreenBase.h"
#include "ClientDataManager.h"
#include "InputValidate.h"
#include "BatchSearchBooleans.h"
#include <limits>

class AddClientScreen : protected ClientScreenBase
{
private:
	const ClientDataManager _manager;

	//string ReadPIN()
	//{
	//	string PIN;
	//	bool isValid = true;
	//	do
	//	{
	//		getline(cin, PIN);
	//		isValid = InputValidate::IsValidPIN(PIN);

	//		if (!isValid)
	//			cout << "\nPlease enter a valid PIN (6 digits, numbers only)\n";

	//	} while (!isValid);

	//	return PIN;
	//}

	//string ReadPhoneNumber()
	//{
	//	string PhoneNumber = "";
	//	bool isValid = true;
	//	do
	//	{
	//		getline(cin, PhoneNumber);
	//		isValid = InputValidate::IsValidPhoneNumber(PhoneNumber);

	//		if (!isValid)
	//			cout << "\nPlease enter a valid Egyptian phone number (11 digits, e.g. 01012345678)!\n";
	//	} while (!isValid);
	//	return PhoneNumber;
	//}

	//long double ReadBalance()
	//{
	//	return InputValidate::ReadFloatBetween(0, numeric_limits<long double>::max());
	//}

	void readClientData(Client& client)
	{

	}


public:
	AddClientScreen(const ClientDataManager& injectedManager)
		:_manager(injectedManager) {}

	void Execute() override
	{
		IScreen::PrintScreenHeader("ADD NEW CLIENT SCREEN");

		do
		{


			
		} while (tolower(InputValidate::DetermineAgain("\nWould you like to add more clients?\n"));
	}
};

