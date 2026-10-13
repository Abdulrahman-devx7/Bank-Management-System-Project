#pragma once
#include <unordered_map>
#include <memory>

#include "IScreen.h"
#include "UserDataManager.h"
#include "ListUsersScreen.h"
#include "AddUserScreen.h"
#include "DeleteUserScreen.h"
#include "UpdateUserScreen.h"
#include "FindUserScreen.h"

class UserManagementMenu : public IScreen
{
private:
    enum class enUserMenuChoice : short
    {
        ListUsers = 1,
        AddNewUser = 2,
        DeleteUser = 3,
        UpdateUser = 4,
        FindUser = 5,
        MainMenu = 6
    };

    User& _activeUser;
    UserDataManager& _userManager;

    std::unordered_map<enUserMenuChoice, std::unique_ptr<IScreen>> _screens;

    void ShowUserManagementMenu()
    {
        PrintScreenHeader("USER MANAGEMENT SCREEN", "Menu");

        std::cout << std::setw(80) << std::left << "" << std::right << std::setw(10) << "[1]" << " List Users\n";
        std::cout << std::setw(80) << std::left << "" << std::right << std::setw(10) << "[2]" << " Add New User\n";
        std::cout << std::setw(80) << std::left << "" << std::right << std::setw(10) << "[3]" << " Delete User\n";
        std::cout << std::setw(80) << std::left << "" << std::right << std::setw(10) << "[4]" << " Update User\n";
        std::cout << std::setw(80) << std::left << "" << std::right << std::setw(10) << "[5]" << " Find User\n";
        std::cout << std::setw(80) << std::left << "" << std::right << std::setw(10) << "[6]" << " Return to Main Menu\n";

        std::cout << std::setw(80) << std::left << "" << UI_LINE_BOUNDS << "\n";
    }

    bool IsChoiceExist(enUserMenuChoice choice)
    {
        return _screens.find(choice) != _screens.end();
    }

public:
    UserManagementMenu(User& activeUser, UserDataManager& userManager)
        : _activeUser(activeUser), _userManager(userManager)
    {
        _screens[enUserMenuChoice::ListUsers] = std::make_unique<ListUsersScreen>(_userManager, activeUser);
        _screens[enUserMenuChoice::AddNewUser] = std::make_unique<AddUserScreen>(_userManager, _activeUser);
        _screens[enUserMenuChoice::DeleteUser] = std::make_unique<DeleteUserScreen>(_userManager, _activeUser);
        _screens[enUserMenuChoice::UpdateUser] = std::make_unique<UpdateUserScreen>(_userManager, _activeUser);
        _screens[enUserMenuChoice::FindUser] = std::make_unique<FindUserScreen>(_userManager, _activeUser);
    }

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
        {
            enUserMenuChoice menuChoice = enUserMenuChoice::MainMenu;
            do
            {
                ResetScreen();
                ShowUserManagementMenu();
                menuChoice = static_cast<enUserMenuChoice>(InputValidate::ReadIntegerBetween(1, 6));

                if (IsChoiceExist(menuChoice))
                {
                    _screens[menuChoice]->Execute();
                    PromptUser("Press any button to go back");
                }
                else
                    menuChoice = enUserMenuChoice::MainMenu;

            } while (menuChoice != enUserMenuChoice::MainMenu);
        }
    }
};