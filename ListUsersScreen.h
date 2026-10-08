#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "User.h"

class ListUsersScreen : protected UserScreenBase
{
private:
    const UserDataManager& _manager;
    const User& _activeUser;

    inline static const int SCREEN_WIDTH = 118;
    inline static const int SCREEN_INDENT = 25;

    static void PrintUsersTableHeader(size_t userCount)
    {
        const std::string indent(SCREEN_INDENT, ' ');

        std::cout << "\n" << indent
            << std::setw((SCREEN_WIDTH + 24) / 2) << std::right
            << "User List: (" + std::to_string(userCount) + ") User(s)\n";

        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";

        // Widths sum: 2 + 16 + 2 + 12 + 2 + 25 + 2 + 28 + 2 + 14 + 2 + 10 + 1 = 118
        std::cout << indent << "| " << std::left
            << std::setw(16) << "Username" << "| "
            << std::setw(12) << "Password" << "| "
            << std::setw(25) << "Full Name" << "| "
            << std::setw(28) << "Email" << "| "
            << std::setw(14) << "Phone Number" << "| "
            << std::setw(10) << "Permission" << "|\n";

        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";
    }

    static void PrintIndividualUserRow(const User& user)
    {
        const std::string indent(SCREEN_INDENT, ' ');

        std::cout << indent << "| " << std::left
            << std::setw(16) << user.Username() << "| "
            << std::setw(12) << user.Password() << "| "
            << std::setw(25) << user.FullName() << "| "
            << std::setw(28) << user.Email() << "| "
            << std::setw(14) << user.PhoneNumber() << "| "
            << std::setw(10) << user.getPermissions() << "|\n";
    }

    void PrintUsersTable()
    {
        const auto& userMap = _manager.GetUserMap();

        PrintUsersTableHeader(userMap.size());

        if (userMap.empty())
        {
            const std::string indent(SCREEN_INDENT, ' ');
            std::cout << indent << "| " << std::left
                << std::setw(SCREEN_WIDTH - 4) << "No users available in the system." << " |\n";
        }
        else
        {
            for (const auto& [username, user] : userMap)
            {
                PrintIndividualUserRow(user);
            }
        }

        const std::string indent(SCREEN_INDENT, ' ');
        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n\n";
    }

public:
    ListUsersScreen(UserDataManager& injectedManager, const User& injectedUser)
        : _manager(injectedManager), _activeUser(injectedUser) {
    }

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
        {
            PrintScreenHeader("LIST USERS SCREEN");
            PrintUsersTable();
        }
    }
};