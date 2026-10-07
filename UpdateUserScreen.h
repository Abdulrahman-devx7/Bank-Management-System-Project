#pragma once
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "InputValidate.h"

class UpdateUserScreen : protected UserScreenBase
{
private:
    UserDataManager _manager;
    User _activeUser;

    void _PrintUpdateUserStatus(UserDataManager::DeleteOrUpdateResult result)
    {
        switch (result)
        {
        case UserDataManager::DeleteOrUpdateResult::Success:
            cout << "\nUser updated successfully!\n";
            break;

        case UserDataManager::DeleteOrUpdateResult::Non_updateMode:
            cout << "\nAn error occurred! Please ensure the user is in a state for update.\n";
            break;

        case UserDataManager::DeleteOrUpdateResult::UserNoExist:
            cout << "\nAn error occurred! The user you're trying to update doesn't exist.\n";
            break;

        default:
            cout << "\nUNDEFINED STATE\n";
            break;
        }
    }

    void _ReadUserDataUpdates(User& targetUser)
    {
        if (toupper(InputValidate::DetermineAgain("\nDo you want to update the username (Y/N)?\n")) == 'Y')
        {
            cout << "Enter username: ";
            targetUser.setUsername(ReadUsername());
        }

        if (toupper(InputValidate::DetermineAgain("\nDo you want to update the password (Y/N)?\n")) == 'Y')
        {
            cout << "Enter password: ";
            targetUser.setPassword(ReadPassword());
        }

        if (toupper(InputValidate::DetermineAgain("\nDo you want to update the full name (Y/N)?\n")) == 'Y')
        {
            cout << "Enter full name: ";
            targetUser.SetFullName(ReadFullName());
        }

        if (toupper(InputValidate::DetermineAgain("\nDo you want to update the email (Y/N)?\n")) == 'Y')
        {
            cout << "Enter email: ";
            targetUser.SetEmail(ReadEmail());
        }

        if (toupper(InputValidate::DetermineAgain("\nDo you want to update the phone number (Y/N)?\n")) == 'Y')
        {
            cout << "Enter phone number: ";
            targetUser.SetPhoneNumber(ReadPhoneNumber());
        }

        if (toupper(InputValidate::DetermineAgain("\nDo you want to update the permissions (Y/N)?\n")) == 'Y')
        {
            _AssignPermissionsToUser(targetUser);
        }
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

    void _UpdateUser(const string& username)
    {
        User targetUser = User::getEmptyUser();
        targetUser.setUsername(username);

        User* foundUser = _manager.Find(targetUser);

        if (foundUser != nullptr)
        {
            PrintInfoCard(*foundUser);

            if (toupper(InputValidate::DetermineAgain("\n\nDo you want to update this user's data (Y/N)?\n")) == 'Y')
            {
                User updatedUser = *foundUser;
                _ReadUserDataUpdates(updatedUser);
                updatedUser.SetMode(User::Mode::Update);

                UserDataManager::DeleteOrUpdateResult result = _manager.Update(*foundUser, updatedUser);
                _PrintUpdateUserStatus(result);
            }
        }
        else
        {
            cout << "\nUser with username (" << username << ") is not found!\n";
        }
    }

public:
    UpdateUserScreen(const UserDataManager& injectedManager, const User& injectedUser)
        : _manager(injectedManager), _activeUser(injectedUser) {}

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
        {
            IScreen::PrintScreenHeader("UPDATE USER SCREEN");

            cout << "\nEnter the username: ";
            string username = ReadUsername();

            _UpdateUser(username);
        }
    }
};
