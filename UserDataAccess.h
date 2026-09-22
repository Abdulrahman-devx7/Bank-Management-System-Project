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

    static void _AddDataLineToFile(string fileName, string dataLine)
    {
        fstream file;
        file.open(fileName, ios::out | ios::app);

        if (file.is_open())
        {
            file << dataLine << endl;
            file.close();
        }
    }

    static User _ConvertUserLineToRecord(string Line, string delimiter = "#//#")
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

    static string _ConvertRecordToLine(User user, string delimiter = "#//#")
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

    static void AppendUserRecord(User &target, const string &filename, const string &delimiter="#//#")
    {
        _AddDataLineToFile(filename, _ConvertRecordToLine(target));

    }

    static enDataStatus LoadData(string fileName, unordered_map<string, User>& clients, string delimiter = "#//#")
    {
        fstream userFile;

        userFile.open(fileName, ios::in);

        if (userFile.is_open())
        {
            string line = "";
            User user = User::getEmptyUser();

            while (getline(userFile, line))
            {
                user = _ConvertUserLineToRecord(line, delimiter);

                if (!user.IsMarkedForDeletion())
                    clients.insert({ user.Username() , user });
            }
            userFile.close();

            if (clients.size() == 0)
                return enDataStatus::eFileEmpty;
            else
                return enDataStatus::eSuccess;

        }
        else
            return enDataStatus::eFileNotFound;
    }

    static void SaveData(string fileName, unordered_map<string, User>& clients)
    {
        fstream userFile;
        userFile.open(fileName, ios::out);

        if(userFile.is_open())
        {
            if (userFile.is_open())
            {
                for (auto& [accountNumber, client] : clients)
                {
                    string line = _ConvertRecordToLine(client);

                    userFile << line << "\n";
                }
                userFile.close();
            }
        }
    }

};

