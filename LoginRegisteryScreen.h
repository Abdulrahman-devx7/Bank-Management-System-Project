#pragma once
#include "IScreen.h"
#include "RegisterLine.h"
#include "Date.h"
#include "Time.h"
#include "User.h"

#include <vector>
#include <iomanip>
#include <string>

class LoginRegistryScreen : public IScreen
{
private:
    vector<RegisterLine>& _loginRegisters;
    User& _activeUser;

    static void PrintFileInfoHeader(const vector<RegisterLine>& registers)
    {
        std::string indent(SCREEN_INDENT, ' ');

        std::cout << "\n" << indent
            << std::setw((SCREEN_WIDTH + 28) / 2) << std::right
            << "Login Register List: (" + std::to_string(registers.size()) + ") Record(s)\n";

        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";

        std::cout << indent << "| " << std::left
            << std::setw(16) << "Date" << " | "
            << std::setw(14) << "Time" << " | "
            << std::setw(24) << "Username" << " | "
            << std::setw(24) << "Password" << " | "
            << std::setw(16) << "Permissions" << " |" << "\n";

        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";
    }

    static void PrintIndividualTableInfo(const RegisterLine& record)
    {
        const std::string indent(SCREEN_INDENT, ' ');

        std::cout << indent << "| " << std::left
            << std::setw(16) << Date::DateToString(record.LoginDate()) << " | "
            << std::setw(14) << Time::TimeToString(record.LoginTime()) << " | "
            << std::setw(24) << record.UserUsername() << " | "
            << std::setw(24) << record.UserPassword() << " | "
            << std::setw(16) << record.UserPermissions() << " |" << "\n";
    }

    void PrintTable()
    {
        PrintFileInfoHeader(_loginRegisters);

        if (_loginRegisters.empty())
        {
            const std::string indent(SCREEN_INDENT, ' ');
            std::cout << indent << "| " << std::left << std::setw(SCREEN_WIDTH - 4) << "No login records found." << " |" << "\n";
            std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";
        }
        else
        {
            for (const auto& record : _loginRegisters)
            {
                PrintIndividualTableInfo(record);
            }
            const std::string indent(SCREEN_INDENT, ' ');
            std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";
        }
    }

public:
    LoginRegistryScreen(vector<RegisterLine>& loginRegisters, User& activeUser)
        : _loginRegisters(loginRegisters), _activeUser(activeUser)
    {
    }

    void Execute() override
    {
        if (!CheckAccessRights(_activeUser, User::Permissions::LoginRegisteryLog))
        {
            return;
        }

        ResetScreen();
        IScreen::PrintScreenHeader("LOGIN REGISTER SCREEN");
        PrintTable();
    }
};