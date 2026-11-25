#pragma once
#include "RichString.h"
#include "Currency.h"
#include <fstream>
#include <unordered_map>

class CurrencyExchangeDataAccess
{
public:
    enum class enDataStatus { eSuccess = 0, eFileNotFound = 1 };

private:

    static void AddDataLineToFile(const string& fileName, const string& dataLine)
    {
        bool needsLeadingNewline = false;
        {
            ifstream checkFile(fileName, ios::binary | ios::ate);
            if (checkFile.is_open() && checkFile.tellg() > 0)
            {
                checkFile.seekg(-1, ios::end);
                char lastChar = 0;
                checkFile.get(lastChar);
                if (lastChar != '\n' && lastChar != '\r')
                    needsLeadingNewline = true;
            }
        }

        fstream file;
        file.open(fileName, ios::out | ios::app);

        if (file.is_open())
        {
            if (needsLeadingNewline)
                file << "\n";
            file << dataLine << endl;
            file.close();
        }
    }

    static Currency ConvertRecordToCurrency(const string& Line, const string& delimiter = "#//#")
    {
        vector<string> CurrencyData = RichString::Split(Line, delimiter);

        if (CurrencyData.size() != 4)
            return Currency::getEmptyCurrency();

        Currency parsedCurrency(Currency::enMode::Update, CurrencyData[0], CurrencyData[1], CurrencyData[2],
            stof(CurrencyData[3]));

        return parsedCurrency;
    }

    static string ConvertCurrencyToRecord(const Currency& currency, const string& delimiter = "#//#")
    {
        string recordLine = "";

        recordLine += currency.GetCountry() + delimiter;
        recordLine += currency.GetCurrencyCode() + delimiter;
        recordLine += currency.GetCurrencyName() + delimiter;
        recordLine += to_string(currency.GetRatePerUSD());

        return recordLine;
    }

public:

    static void AppendCurrencyRecord(const Currency& target, const string& filename, const string& delimiter = "#//#")
    {
        AddDataLineToFile(filename, ConvertCurrencyToRecord(target, delimiter));
    }

    static enDataStatus LoadData(
        const string& fileName,
        unordered_map<string, Currency>& currenciesByCode,
        unordered_map<string, Currency*>& currenciesByCountry,
        const string& delimiter = "#//#")
    {
        fstream currencyFile;
        currencyFile.open(fileName, ios::in);

        if (currencyFile.is_open())
        {
            string line = "";
            Currency currency = Currency::getEmptyCurrency();

            while (getline(currencyFile, line))
            {
                if (line.empty())
                    continue;

                currency = ConvertRecordToCurrency(line, delimiter);

                if (!currency.IsEmpty())
                {
                    auto [it, inserted] = currenciesByCode.insert({ currency.GetCurrencyCode(), currency });
                    currenciesByCountry.insert({ currency.GetCountry(), &(it->second) });
                }
            }
            currencyFile.close();
            return enDataStatus::eSuccess;
        }
        else
            return enDataStatus::eFileNotFound;
    }

    static void SaveData(const string& fileName, const unordered_map<string, Currency>& currencies)
    {
        fstream currencyFile;
        currencyFile.open(fileName, ios::out);

        if (currencyFile.is_open())
        {
            for (const auto& [code, currency] : currencies)
            {
                string line = ConvertCurrencyToRecord(currency);

                currencyFile << line << "\n";
            }
            currencyFile.close();
        }
    }

};
