#pragma once
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "InputValidate.h"

class DeleteUserScreen : protected UserScreenBase
{
private:
    UserDataManager _manager;
    User _activeUser;

    void _PrintDeleteUserStatus(UserDataManager::DeleteOrUpdateResult result)
    {
        switch (result)
        {
        case UserDataManager::DeleteOrUpdateResult::Success:
            cout << "\nUser marked for deletion successfully!\n";
            break;

        case UserDataManager::DeleteOrUpdateResult::Non_updateMode:
            cout << "\nAn error occurred! Please ensure the user is in a state for deletion.\n";
            break;

        case UserDataManager::DeleteOrUpdateResult::UserNoExist:
            cout << "\nAn error occurred! The user you're trying to delete doesn't exist.\n";
            break;

        default:
            cout << "\nUNDEFINED STATE\n";
            break;
        }
    }

    void _DeleteUser(const string& username)
    {
        User targetUser = User::getEmptyUser();
        targetUser.setUsername(username);

        User* foundUser = _manager.Find(targetUser);

        if (foundUser != nullptr)
        {
            PrintInfoCard(*foundUser);

            if (toupper(InputValidate::DetermineAgain("\n\nAre you sure you want to delete this user (Y/N)?\n")) == 'Y')
            {
                User userToDelete = *foundUser;
                UserDataManager::DeleteOrUpdateResult result = _manager.Delete(userToDelete);
                _PrintDeleteUserStatus(result);
            }
        }
        else
        {
            cout << "\nUser with username (" << username << ") is not found!\n";
        }
    }

public:
    DeleteUserScreen(const UserDataManager& injectedManager, const User& injectedUser)
        : _manager(injectedManager), _activeUser(injectedUser) {}

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
        {
            IScreen::PrintScreenHeader("DELETE USER SCREEN");

            cout << "\nEnter the username: ";
            string username = ReadUsername();

            _DeleteUser(username);
        }
    }
};
