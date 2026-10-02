#pragma once
struct SearchFilterOptions
{
    bool matchAccountNumber = false;
    bool matchEmail = false;
    bool matchPhoneNumber = false;

    bool hasAnyMatch() const
    {
        return matchAccountNumber || matchEmail || matchPhoneNumber;
    }
};