#pragma once
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "InputValidate.h"
#include "UserBatchSearch.h"

class UpdateUserScreen : public UserScreenBase
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
        UserBatchSearch result;
        do
        {
            _handleBatchSearchError(result);

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

            if (toupper(InputValidate::DetermineAgain("\nDo you want to update the username (Y/N)?\n")) == 'Y')
            {
                cout << "Enter username: ";
                targetUser.setUsername(ReadUsername());
            }
            result = _manager.BatchSearch(targetUser.Username(), targetUser.Email(), targetUser.PhoneNumber());

        } while (result.hasAnyMatch());

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


        if (toupper(InputValidate::DetermineAgain("\nDo you want to update the permissions (Y/N)?\n")) == 'Y')
        {
            AssignPermissionsToUser(targetUser);
        }
    }

    void _UpdateUser(const string& username)
    {
        User* foundUser = nullptr;

        if ((foundUser = (_manager.Find(username))) != nullptr)
        {
            PrintInfoCard(*foundUser);

            if (toupper(InputValidate::DetermineAgain("\n\nDo you want to update this client's data (Y/N)?\n")) == 'Y')
            {
                _ReadUserDataUpdates(*foundUser);
                _manager.Update(*foundUser);
            }

        }
        else
            cout << "\nClient with account number (" << username << ") is not found!\n";

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
