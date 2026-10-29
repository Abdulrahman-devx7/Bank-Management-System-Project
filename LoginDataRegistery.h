#pragma once
#include "User.h"
#include "RichString.h"
#include "RegisterLine.h"
#include <unordered_map>
#include <fstream>
#include <vector>

class LoginDataRegistery
{
public:
	enum class enDataStatus { eSuccess = 0, eFileNotFound = 1, eFileEmpty = 2 };

private:

    static RegisterLine registerLine;

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

    static RegisterLine _ConvertRecordToRegisterLine(const string& line, const string& delimiter = "#//#")
    {
        vector<string> RegisterData = RichString::Split(line, delimiter);
        vector<string> timeData = RichString::Split(RegisterData[0], "-");

        Date loginDate = Date::StringToDate(timeData[0]);
        Time loginTime = Time::StringToTime(timeData[1]);
        string username = RegisterData[1];
        string password = RegisterData[2];
        int32_t permissions = stoi(RegisterData[3]);

        return RegisterLine(loginDate, loginTime, username, password, permissions);
    }

    static string _ConvertRegisterLineToRecord(const RegisterLine& registerData, const string& delimiter = "#//#")
    {
        string recordLine;

        recordLine += Date::DateToString(registerData.LoginDate()) + "-";
        recordLine += Time::TimeToString(registerData.LoginTime()) + delimiter;
        recordLine += registerData.UserUsername() + delimiter;
        recordLine += registerData.UserPassword() + delimiter;
        recordLine += to_string(registerData.UserPermissions());

        return recordLine;
    }

public:
    
    static void RecordLogin(const string &fileName, const User &loggedUser)
    {
        RegisterLine line(Date::GetSystemDate(), Time::GetSystemTime(), loggedUser.Username(), loggedUser.Password(), loggedUser.getPermissions());
        _AddDataLineToFile(fileName, _ConvertRegisterLineToRecord(line));
    }

    static enDataStatus LoadLog(const string& fileName, vector<RegisterLine> &log, const string& delimiter = "#//#")
    {
        fstream logFile;

        logFile.open(fileName, ios::in);

        if (logFile.is_open())
        {
            string line = "";
            RegisterLine registerData = RegisterLine::GetEmptyRegisterLine();

            while (getline(logFile, line))
            {
                if (line.empty())
                    continue;

                registerData = _ConvertRecordToRegisterLine(line, delimiter);
                log.push_back(registerData);

            }
            logFile.close();

            if (log.size() == 0)
                return enDataStatus::eFileEmpty;
            else
                return enDataStatus::eSuccess;

        }
        else
            return enDataStatus::eFileNotFound;
    }

    static void SaveLog(const string &fileName, const vector<RegisterLine>& log)
    {
        fstream logFile;
        logFile.open(fileName, ios::out);

        if (logFile.is_open())
        {
            for (const auto& registerAttempt : log)
            {
                string line = _ConvertRegisterLineToRecord(registerAttempt);

                logFile << line << "\n";
            }
            logFile.close();
        }
    }

};

