#pragma once
#include <iostream>
#include <string>
#include <iomanip>

#include "User.h"

using namespace std;

class IScreen
{
protected:

    static void PrintScreenHeader(string ScreenTitle, string SubTitle = "")
    {
        cout << setw(80) << left << "" << string(45, '-') << "\n";
        cout << setw(80) << left << "" << right << setw(30) << ScreenTitle << "\n";
        cout << setw(80) << left << "" << string(45, '-') << "\n\n";

        if (SubTitle != "")
        {
            cout << setw(80) << left << "" << string(45, '-') << "\n";
            cout << setw(80) << left << "" << right << setw(30) << SubTitle << "\n";
            cout << setw(80) << left << "" << string(45, '-') << "\n";
        }
    }

    static bool CheckAccessRights(User &activeUser, User::Permissions screenPermission)
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
    virtual void Execute() = 0;   
    virtual ~IScreen() = default;

};

