#pragma once
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "InputValidate.h"

class AddUserScreen : protected UserScreenBase
{
private:
    UserDataManager _manager;
    User _activeUser;

    void _ReadUserData(User& user)
    {
        cout << "\nEnter username: ";
        user.setUsername(ReadUsername());

        cout << "\nEnter password: ";
        user.setPassword(ReadPassword());

        cout << "\nEnter full name: ";
        user.SetFullName(ReadFullName());

        cout << "\nEnter email: ";
        user.SetEmail(ReadEmail());

        cout << "\nEnter phone number: ";
        user.SetPhoneNumber(ReadPhoneNumber());

        _AssignPermissionsToUser(user);

        user.SetMode(User::Mode::New);
    }

    void _AssignPermissionsToUser(User& user)
    {
        if (toupper(InputValidate::DetermineAgain("\nDo you want to give this user full access (Y/N)?\n")) == 'Y')
        {
            user.setPermissions(-1);
            return;
        }

        cout << "\nDo you want to give access to:\n\n";

        int permissions = 0;

        if (toupper(InputValidate::DetermineAgain("Do you want to give this user access to showing client list (Y/N)?\n")) == 'Y')
            permissions = permissions | static_cast<int>(User::Permissions::ListClients);

        cout << "\n";

        if (toupper(InputValidate::DetermineAgain("Do you want to give this user access to adding new clients (Y/N)?\n")) == 'Y')
            permissions = permissions | static_cast<int>(User::Permissions::AddNewClient);

        cout << "\n";

        if (toupper(InputValidate::DetermineAgain("Do you want to give this user access to deleting clients (Y/N)?\n")) == 'Y')
            permissions = permissions | static_cast<int>(User::Permissions::DeleteClient);

        cout << "\n";

        if (toupper(InputValidate::DetermineAgain("Do you want to give this user access to updating clients (Y/N)?\n")) == 'Y')
            permissions = permissions | static_cast<int>(User::Permissions::UpdateClients);

        cout << "\n";

        if (toupper(InputValidate::DetermineAgain("Do you want to give this user access to finding clients (Y/N)?\n")) == 'Y')
            permissions = permissions | static_cast<int>(User::Permissions::FindClient);

        cout << "\n";

        if (toupper(InputValidate::DetermineAgain("Do you want to give this user access to transactions (Y/N)?\n")) == 'Y')
            permissions = permissions | static_cast<int>(User::Permissions::Transactions);

        cout << "\n";

        if (toupper(InputValidate::DetermineAgain("Do you want to give this user access to managing users (Y/N)?\n")) == 'Y')
            permissions = permissions | static_cast<int>(User::Permissions::ManageUsers);

        cout << "\n";

        user.setPermissions(permissions);
    }

    void _PrintAddUserStatus(UserDataManager::enSaveResult result)
    {
        switch (result)
        {
        case UserDataManager::enSaveResult::Success:
            cout << "\nUser added successfully!\n";
            break;

        case UserDataManager::enSaveResult::Non_AddNewMode:
            cout << "\nAn error occurred! Please ensure the user is in a state for addition.\n";
            break;

        case UserDataManager::enSaveResult::UserExists:
            cout << "\nAn error occurred! A user with this username already exists.\n";
            break;

        default:
            cout << "\nUNDEFINED STATE\n";
            break;
        }
    }

public:
    AddUserScreen(const UserDataManager& injectedManager, const User& injectedUser)
        : _manager(injectedManager), _activeUser(injectedUser) {}

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
        {
            IScreen::PrintScreenHeader("ADD NEW USER SCREEN");

            do
            {
                User newUser = User::getEmptyUser();
                _ReadUserData(newUser);

                UserDataManager::enSaveResult result = _manager.Add(newUser);
                _PrintAddUserStatus(result);

            } while (toupper(InputValidate::DetermineAgain("\nWould you like to add more users (Y/N)?\n")) == 'Y');
        }
    }
};
