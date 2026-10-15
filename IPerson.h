#pragma once
#include <string>
using namespace std;
class IPerson
{
private:
    string _fullName;
    string _email;
    string _phoneNumber;
protected:
    IPerson(const string& fullName, const string& email, const string& phoneNumber) :
        _fullName(fullName), _email(email), _phoneNumber(phoneNumber){
    }

public:
    // Getters
    const string& FullName() const
    {
        return _fullName;
    }

    const string& Email() const
    {
        return _email;
    }

    const string& PhoneNumber() const
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

};

