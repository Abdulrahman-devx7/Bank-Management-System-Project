#pragma once
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "InputValidate.h"
#include "UserBatchSearch.h"

class AddUserScreen : public UserScreenBase
{
private:
    UserDataManager& _manager;
    User& _activeUser;

    void _ReadUserData(User& user)
    {
        UserBatchSearch result;
        string pad = GetIndentPadding();

        do
        {
            _handleBatchSearchError(result);

            cout << pad << "Enter email: ";
            user.SetEmail(ReadEmail());

            cout << "\n" << pad << "Enter phone number: ";
            user.SetPhoneNumber(ReadPhoneNumber());

            cout << "\n" << pad << "Enter username: ";
            user.setUsername(ReadUsername());

            result = _manager.BatchSearch(user.Username(), user.Email(), user.PhoneNumber());

        } while (result.hasAnyMatch());

        cout << "\n" << pad << "Enter password: ";
        user.setPassword(ReadPassword());

        cout << "\n" << pad << "Enter full name: ";
        user.SetFullName(ReadFullName());

        AssignPermissionsToUser(user);

        user.SetMode(User::Mode::New);
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
    AddUserScreen(UserDataManager& injectedManager, User& injectedUser)
        : _manager(injectedManager), _activeUser(injectedUser) {}

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
        {
            ResetScreen();
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
