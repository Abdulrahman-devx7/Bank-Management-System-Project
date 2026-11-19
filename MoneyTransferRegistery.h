#pragma once
#include "User.h"
#include "RichString.h"
#include "TransferRecordStruct.h"
#include <unordered_map>
#include <fstream>
#include <vector>

class MoneyTransferRegistery
{
	TransferRecordStruct _record;

    static void _AddDataLineToFile(const string& fileName, const string& dataLine)
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

    static TransferRecordStruct _ConvertRecordToTransferToDTO(const string& line, const string delimiter = "#//#")
    {
        vector<string> registerData = RichString::Split(line, delimiter);
        vector<string> timeData = RichString::Split(registerData[0], "-");

        Date transferDate = Date::StringToDate(timeData[0]);
        Time transferTime = Time::StringToTime(timeData[1]);

        string sender = registerData[1];
        string receiver = registerData[2];

        float amount = stod(registerData[3]);
        float senderBalance = stod(registerData[4]);
        float receiverBalance = stod(registerData[5]);

        string Executor = registerData[6];

        return TransferRecordStruct(transferDate, transferTime, sender, receiver, amount, senderBalance, receiverBalance, Executor);

    }

    static string _ConvertRegisterLineToRecord(const TransferRecordStruct& registerData, const string& delimiter = "#//#")
    {
        string recordLine;

        recordLine += Date::DateToString(registerData.transferDate) + "-";
        recordLine += Time::TimeToString(registerData.transferTime) + delimiter;
        recordLine += registerData.senderAccountNumber + delimiter;
        recordLine += registerData.receiverAccountNumber + delimiter;
        recordLine += to_string(registerData.transferredAmount) + delimiter;
        recordLine += to_string(registerData.senderBalance) + delimiter;
        recordLine += to_string(registerData.receiverBalance) + delimiter;
        recordLine += registerData.transferExcecutor;

        return recordLine;
    }

public:

    static void RecordTransfer(const string& fileName, const TransferRecordStruct& transfer)
    {
        _AddDataLineToFile(fileName, _ConvertRegisterLineToRecord(transfer));
    }

    static void LoadTransferLog(const string& fileName, vector<TransferRecordStruct>& log, const string& delimiter = "#//#")
    {
        fstream logFile;

        logFile.open(fileName, ios::in | ios::out | ios::app);

        if (logFile.is_open())
        {
            string line = "";
            TransferRecordStruct transferRecord;

            while (getline(logFile, line))
            {
                if (line.empty())
                    continue;

                transferRecord = _ConvertRecordToTransferToDTO(line, delimiter);
                log.push_back(transferRecord);
            }
            logFile.close();
        }
    }

    static void SaveTransferLog(const string& fileName, const vector<TransferRecordStruct>& log)
    {
        fstream logFile;
        logFile.open(fileName, ios::out);

        if (logFile.is_open())
        {
            for (const auto& transferRecord : log)
            {
                string line = _ConvertRegisterLineToRecord(transferRecord);
                logFile << line << "\n";
            }
            logFile.close();
        }
    }



};

