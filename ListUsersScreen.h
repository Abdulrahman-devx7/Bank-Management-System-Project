#pragma once
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "InputValidate.h"

class ListUsersScreen : protected UserScreenBase
{
private:
    UserDataManager _manager;
    User _activeUser;

    void _PrintUsersList()
    {
        const int SCREEN_WIDTH = 125;
        const string UI_LINE_BOUNDS(44, '=');

        cout << setw(55) << left << "" << right << setw(50) << "User list: (" << _manager.GetUsersCount() << ") user(s)" << "\n";
        cout << setw(65) << left << "" << string(77, '-') << "\n";
        cout << setw(65) << left << "" << left << "| " << setw(20) << " User Name " << "| ";
        cout << setw(30) << " Full Name " << "| ";
        cout << setw(20) << " Permissions " << "|" << "\n";
        cout << setw(65) << left << "" << string(77, '-') << "\n";

        _manager.PrintAllUsers();

        cout << setw(65) << left << "" << string(77, '-') << "\n";
    }

public:
    ListUsersScreen(const UserDataManager& injectedManager, const User& injectedUser)
        : _manager(injectedManager), _activeUser(injectedUser) {}

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
        {
            IScreen::PrintScreenHeader("LIST USERS SCREEN");
            _PrintUsersList();
        }
    }
};
