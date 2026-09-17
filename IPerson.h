#pragma once
#include <string>
using namespace std;
class IPerson
{
public:
    enum class Mode { Empty = 1, AddNew = 2, Update = 3 };

private:
    string _fullName;
    string _email;
    string _phoneNumber;
    Mode _mode;

public:

    // Getters
    string FullName() const
    {
        return _fullName;
    }

    string Email() const
    {
        return _email;
    }

    string PhoneNumber() const
    {
        return _phoneNumber;
    }

    // Setters
    void SetFullName(const string& fullName)
    {
        _fullName = fullName;
    }

    void SetEmail(const string& email)
    {
        _email = email;
    }

    void SetPhoneNumber(const string& phoneNumber)
    {
        _phoneNumber = phoneNumber;
    }

    IPerson(string fullName, string email, string phoneNumber) :
        _fullName(fullName), _email(email), _phoneNumber(phoneNumber) {}

    virtual void PrintInfoCard() = 0;
};

