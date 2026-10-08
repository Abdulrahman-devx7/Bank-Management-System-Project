#pragma once
struct UserBatchSearch
{
    bool matchUsername = false;
    bool matchEmail = false;
    bool matchPhoneNumber = false;

    bool hasAnyMatch() const
    {
        return matchUsername || matchEmail || matchPhoneNumber;
    }
};
