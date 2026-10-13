#pragma once
#include "IScreen.h"
#include "User.h"
#include "RichString.h"
#include "InputValidate.h"
#include <string>

class UserScreenBase : public IScreen
{
private:
    static string _readUsername()
    {
        string username = "";
        getline(cin >> ws, username);
        return username;
    }

    static string _readPassword()
    {
        string password = "";
        getline(cin >> ws, password);
        return password;
    }

    static string _readEmail()
    {
        string email = "";
        getline(cin >> ws, email);
        return email;
    }

    static string _readPhoneNumber()
    {
        string phoneNumber = "";
        getline(cin, phoneNumber);
        return phoneNumber;
    }

    static string _readFullName()
    {
        string fullName = "";
        getline(cin, fullName);
        return fullName;
    }

    static void _HandleBadUsername(bool hasSpaces)
    {
        if (hasSpaces)
            cout << "\nPlease, enter a valid username without any spaces!\nThe username should contain only letters, numbers, and special characters.\n";
    }

    static void _HandleBadPhoneNumber(bool isValid)
    {
        if (!isValid)
            cout << "\nPlease enter a valid Egyptian phone number (11 digits, e.g. 01012345678)!\n";
    }

    static void _HandleBadFullName(bool isValid)
    {
        if (!isValid)
            cout << "\nPlease, enter a valid full name form consisting of 4 names without any number or special characters!\n\n";
    }

protected:

    UserScreenBase() {}

    void _handleBatchSearchError(const UserBatchSearch& options)
    {
        if (options.hasAnyMatch())
        {
            cout << "\nAn error occurred! The following field/s are pre-existent in our database:\n\n";
            if (options.matchUsername)
                cout << "- The Username\n";

            if (options.matchEmail)
                cout << "- The Email\n";

            if (options.matchPhoneNumber)
                cout << "- The Phone Number";

            cout << "\n\nPlease, renter these credentials: \n\n";
        }
    }

    string ReadUsername()
    {
        string username = "";
        bool containsSpaces = false;

        do
        {
            username = _readUsername();
            InputValidate::NormalizeUsername(username);
            containsSpaces = InputValidate::CheckSpacesInUsername(username);

            _HandleBadUsername(containsSpaces);

        } while (containsSpaces);

        return username;
    }

    string ReadPassword()
    {
        return _readPassword();
    }

    string ReadEmail()
    {
        return _readEmail();
    }

    string ReadPhoneNumber()
    {
        string phoneNumber = "";
        bool isValid = true;

        do
        {
            phoneNumber = _readPhoneNumber();
            isValid = InputValidate::IsValidPhoneNumber(phoneNumber);
            _HandleBadPhoneNumber(isValid);

        } while (!isValid);

        return phoneNumber;
    }

    string ReadFullName()
    {
        bool is4Names = true;
        bool isAllLetters = true;
        vector<string> words;

        do
        {
            string fullName = _readFullName();
            words = RichString::Split(fullName, " ");

            is4Names = (words.size() == 4);
            isAllLetters = InputValidate::AreNamesOnlyLetters(words);

            _HandleBadFullName(!is4Names || !isAllLetters);

        } while (!isAllLetters || !is4Names);

        return RichString::JoinString(words, " ");
    }

    void AssignPermissionsToUser(User& user)
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

    void PrintInfoCard(const User& user)
    {
        const int indentWidth = 40;
        const int labelWidth = 18;
        const std::string indent(indentWidth, ' ');

        std::cout << "\n" << indent << UI_LINE_BOUNDS << "\n";
        std::cout << indent << std::setw(labelWidth + 12) << "User Card Details" << "\n";
        std::cout << indent << UI_LINE_BOUNDS << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Username"
            << ": " << user.Username() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Password"
            << ": " << user.Password() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Full Name"
            << ": " << user.FullName() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Email"
            << ": " << user.Email() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Phone Number"
            << ": " << user.PhoneNumber() << "\n";

        std::cout << indent << std::left << std::setw(labelWidth) << "Permissions"
            << ": " << user.getPermissions() << "\n";

        std::cout << indent << UI_LINE_BOUNDS << "\n\n";
    }

};
