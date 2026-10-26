#pragma once
#include <iostream>
#include <string>
#include <iomanip>

#include "User.h"
#include "Date.h"

using namespace std;

class IScreen
{
private:
    inline static User _currentActiveUser = User::getEmptyUser();

protected:

    inline static const std::string UI_LINE_BOUNDS = std::string(44, '=');    
    inline static const int SCREEN_WIDTH = 118;
    inline static const int SCREEN_INDENT = 25;

    static bool CheckAccessRights(const User &activeUser, const User::Permissions screenPermission)
    {
        if (activeUser.CheckAccessPermission(screenPermission))
            return true;

        else
        {
            PrintScreenHeader("ACCESS DENIED", "You currently have no access\n to this page.\nPlease, contact your admin.");
            return false;
        }
    }

public:
    static void SetActiveUser(const User& user)
    {
        _currentActiveUser = user;
    }

    static void ClearActiveUser()
    {
        _currentActiveUser = User::getEmptyUser();
    }

    static void PrintScreenHeader(const string& ScreenTitle, const string& SubTitle = "")
    {
        cout << setw(80) << left << "" << string(45, '-') << "\n";
        cout << setw(80) << left << "" << right << setw(30) << ScreenTitle << "\n";
        cout << setw(80) << left << "" << string(45, '-') << "\n";

        if (SubTitle != "")
        {
            cout << setw(80) << left << "" << right << setw(30) << SubTitle << "\n";
            cout << setw(80) << left << "" << string(45, '-') << "\n";
        }
        cout << "\n";

        cout << setw(80) << left << "" << "User: " << _currentActiveUser.Username() << "\n";

        cout << setw(80) << left << "" << "Date: " << Date::FormatDate(Date::GetSystemDate()) << "\n";
        cout << setw(80) << left << "" << string(45, '-') << "\n";
    }

    static void PromptUser(const string& message)
    {
        cout << "\n\n" << message;
        system("pause>0");
    }

    static void ResetScreen()
    {
        system("cls");
        system("color 0f");
    }

    virtual void Execute() = 0;   
    virtual ~IScreen() = default;

};
