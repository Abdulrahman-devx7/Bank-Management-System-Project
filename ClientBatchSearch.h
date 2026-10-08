#pragma once
struct ClientBatchSearch
{
    bool matchAccountNumber = false;
    bool matchEmail = false;
    bool matchPhoneNumber = false;

    bool hasAnyMatch() const
    {
        return matchAccountNumber || matchEmail || matchPhoneNumber;
    }
};