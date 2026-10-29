#pragma once
#include <string>
#include "Date.h"
#include "Time.h"

using namespace std;

class RegisterLine
{
private:
    Date loginDate;
    Time loginTime;
    string userUsername;
    string userPassword;
    int32_t userPermissions = 0;

public:
    // Constructor
    RegisterLine(Date loginDate, Time loginTime, string userUsername, string userPassword, int32_t userPermissions)
        : loginDate(loginDate), loginTime(loginTime), userUsername(userUsername), userPassword(userPassword), userPermissions(userPermissions)
    {
    }

    // Setters
    void SetLoginDate(Date loginDate)
    {
        this->loginDate = loginDate;
    }

    void SetLoginTime(Time loginTime)
    {
        this->loginTime = loginTime;
    }

    void SetUserUsername(string userUsername)
    {
        this->userUsername = userUsername;
    }

    void SetUserPassword(string userPassword)
    {
        this->userPassword = userPassword;
    }

    void SetUserPermissions(int32_t userPermissions)
    {
        this->userPermissions = userPermissions;
    }

    // Getters

    static RegisterLine GetEmptyRegisterLine()
    {
        return RegisterLine(Date::GetSystemDate(), Time::GetSystemTime(), "", "", 0);
    }

    Date LoginDate() const
    {
        return loginDate;
    }

    Time LoginTime() const
    {
        return loginTime;
    }

    string UserUsername() const
    {
        return userUsername;
    }

    string UserPassword() const
    {
        return userPassword;
    }

    int32_t UserPermissions() const
    {
        return userPermissions;
    }
};