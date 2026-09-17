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

protected:
    IPerson(string fullName, string email, string phoneNumber) :
        _fullName(fullName), _email(email), _phoneNumber(phoneNumber) {
    }

    // Overload that allows initializing the mode as well
    IPerson(Mode mode, string fullName, string email, string phoneNumber) :
        _fullName(fullName), _email(email), _phoneNumber(phoneNumber), _mode(mode) {
    }

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

    Mode Status()
    {
        return _mode;
    }

    bool IsEmpty()
    {
        return _mode == Mode::Empty;
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

