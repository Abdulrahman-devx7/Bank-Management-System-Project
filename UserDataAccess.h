#pragma once
#include "RichString.h"
#include "User.h" 
#include <fstream>
#include <unordered_map>

class UserDataAccess
{
public:
    enum class enDataStatus { eSuccess = 0, eFileNotFound = 1, eFileEmpty = 2 };

private:

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

    static User _ConvertUserLineToRecord(const string& Line, const string& delimiter = "#//#")
    {
        vector<string> UserData = RichString::Split(Line, delimiter);

        if (UserData.size() != 7)
            return User::getEmptyUser();

        User parsedUser(User::Mode::Update, UserData[0], UserData[1], UserData[2],
            UserData[3], UserData[4], stoi(UserData[5]));

        if (UserData[6] == "1")
            parsedUser.MarkForDeletion();

        return parsedUser;

    }

    static string _ConvertRecordToLine(const User& user, const string& delimiter = "#//#")
    {
        string recordLine = "";

        recordLine += user.FullName() + delimiter;
        recordLine += user.Email() + delimiter;
        recordLine += user.PhoneNumber() + delimiter;

        recordLine += user.Username() + delimiter;
        recordLine += user.Password() + delimiter;
        recordLine += to_string(user.getPermissions()) + delimiter;
        recordLine += (user.IsMarkedForDeletion() ? "1" : "0");

        return recordLine;

    }

public:

    static void AppendUserRecord(const User& target, const string& filename, const string& delimiter="#//#")
    {
        _AddDataLineToFile(filename, _ConvertRecordToLine(target, delimiter));

    }

    static enDataStatus LoadData(const string& fileName, unordered_map<string, User>& users, const string& delimiter = "#//#")
    {
        fstream userFile;

        userFile.open(fileName, ios::in);

        if (userFile.is_open())
        {
            string line = "";
            User user = User::getEmptyUser();

            while (getline(userFile, line))
            {
                if (line.empty())
                    continue;

                user = _ConvertUserLineToRecord(line, delimiter);

                if (!user.IsEmpty() && !user.IsMarkedForDeletion())
                    users.insert({ user.Username() , user });
            }
            userFile.close();

            if (users.size() == 0)
                return enDataStatus::eFileEmpty;
            else
                return enDataStatus::eSuccess;

        }
        else
            return enDataStatus::eFileNotFound;
    }

    static void SaveData(const string& fileName, const unordered_map<string, User>& clients)
    {
        fstream userFile;
        userFile.open(fileName, ios::out);

        if(userFile.is_open())
        {
            for (const auto& [accountNumber, client] : clients)
            {
                string line = _ConvertRecordToLine(client);

                userFile << line << "\n";
            }
            userFile.close();
        }
    }

};

