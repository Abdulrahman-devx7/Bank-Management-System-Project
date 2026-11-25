#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include "CurrencyExchangeScreenBase.h"
#include "CurrencyExchangeManager.h"
#include "RichString.h"
#include "User.h"

class CurrenciesListScreen : public CurrencyExchangeScreenBase
{
private:
    CurrencyExchangeManager& _manager;
    User& _activeUser;

    inline static const int TABLE_WIDTH = 100;
    inline static const int TABLE_INDENT = 52;

    static void PrintCurrenciesHeader()
    {
        const std::string indent(TABLE_INDENT, ' ');

        std::cout << indent << std::string(TABLE_WIDTH, '_') << "\n\n";

        std::cout << indent << "| " << std::left
            << std::setw(32) << "Country" << "| "
            << std::setw(8) << "Code" << "| "
            << std::setw(40) << "Name" << "| "
            << std::setw(12) << "Rate/(1$)" << "\n";

        std::cout << indent << std::string(TABLE_WIDTH, '_') << "\n\n";
    }

    static void PrintIndividualCurrencyRow(const Currency& cur)
    {
        const std::string indent(TABLE_INDENT, ' ');

        std::cout << indent << "| " << std::left
            << std::setw(32) << RichString::StartWordsWithUppercase(cur.GetCountry(), " ") << "| "
            << std::setw(8)  << RichString::ToUpper(cur.GetCurrencyCode()) << "| "
            << std::setw(40) << cur.GetCurrencyName() << "| "
            << std::setw(12) << cur.GetRatePerUSD() << "\n";
    }

    void PrintCurrenciesTable()
    {
        const auto& currencyMap = _manager.GetCurrencyCodeMap();

        PrintCurrenciesHeader();

        if (currencyMap.empty())
        {
            const std::string indent(TABLE_INDENT, ' ');
            std::cout << indent << "| " << std::left
                << std::setw(TABLE_WIDTH - 4) << "No currencies available in the system." << " |\n";
        }
        else
        {
            for (const auto& [code, currency] : currencyMap)
            {
                PrintIndividualCurrencyRow(currency);
            }
        }

        const std::string indent(TABLE_INDENT, ' ');
        std::cout << indent << std::string(TABLE_WIDTH, '_') << "\n\n";
    }

public:
    CurrenciesListScreen(CurrencyExchangeManager& manager, User& activeUser)
        : _manager(manager), _activeUser(activeUser) {}

    void Execute() override
    {
        ResetScreen();
        const auto& currencyMap = _manager.GetCurrencyCodeMap();
        PrintScreenHeader("Currencies List Screen", "(" + std::to_string(currencyMap.size()) + ") Currencies.");
        PrintCurrenciesTable();
    }
};
