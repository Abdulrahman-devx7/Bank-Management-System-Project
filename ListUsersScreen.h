#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include "UserScreenBase.h"
#include "UserDataManager.h"
#include "User.h"

class ListUsersScreen : public UserScreenBase
{
private:
    const UserDataManager& _manager;
    const User& _activeUser;

    inline static const int USER_TABLE_WIDTH = 132;

    static void PrintUsersTableHeader(size_t userCount)
    {
        int indent = GetCenterIndent(USER_TABLE_WIDTH);
        const std::string pad(indent, ' ');

        std::cout << "\n" << pad
            << CenterText("User List: (" + std::to_string(userCount) + ") User(s)", USER_TABLE_WIDTH) << "\n";

        std::cout << pad << std::string(USER_TABLE_WIDTH, '-') << "\n";

        // Widths sum: 2 + 16 + 2 + 15 + 2 + 32 + 2 + 30 + 2 + 14 + 2 + 12 + 1 = 132
        std::cout << pad << "| " << std::left
            << std::setw(16) << "Username" << "| "
            << std::setw(15) << "Password" << "| "
            << std::setw(32) << "Full Name" << "| "
            << std::setw(30) << "Email" << "| "
            << std::setw(14) << "Phone Number" << "| "
            << std::setw(12) << "Permission" << "|\n";

        std::cout << pad << std::string(USER_TABLE_WIDTH, '-') << "\n";
    }

    static void PrintIndividualUserRow(const User& user)
    {
        int indent = GetCenterIndent(USER_TABLE_WIDTH);
        const std::string pad(indent, ' ');

        std::cout << pad << "| " << std::left
            << std::setw(16) << FormatCell(user.Username(), 16) << "| "
            << std::setw(15) << FormatCell(user.Password(), 15) << "| "
            << std::setw(32) << FormatCell(user.FullName(), 32) << "| "
            << std::setw(30) << FormatCell(user.Email(), 30) << "| "
            << std::setw(14) << FormatCell(user.PhoneNumber(), 14) << "| "
            << std::setw(12) << user.getPermissions() << "|\n";
    }

    void PrintUsersTable()
    {
        const auto& userMap = _manager.GetUserMap();

        PrintUsersTableHeader(userMap.size());

        int indent = GetCenterIndent(USER_TABLE_WIDTH);
        const std::string pad(indent, ' ');

        if (userMap.empty())
        {
            std::cout << pad << "| " << std::left
                << std::setw(USER_TABLE_WIDTH - 4) << "No users available in the system." << " |\n";
        }
        else
        {
            for (const auto& [username, user] : userMap)
            {
                PrintIndividualUserRow(user);
            }
        }

        std::cout << pad << std::string(USER_TABLE_WIDTH, '-') << "\n\n";
    }

public:
    ListUsersScreen(UserDataManager& injectedManager, const User& injectedUser)
        : _manager(injectedManager), _activeUser(injectedUser) {
    }

    void Execute() override
    {
        if (CheckAccessRights(_activeUser, User::Permissions::ManageUsers))
        {
            ResetScreen();
            PrintScreenHeader("LIST USERS SCREEN");
            PrintUsersTable();
        }
    }
};