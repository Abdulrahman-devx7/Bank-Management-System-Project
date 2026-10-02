#pragma once
#include "IScreen.h"

class ClientScreenBase
{
protected:
    string ReadAccountNumber()
    {
        string userInput = "";
        getline(cin >> ws, userInput);

        return userInput;
    }

};

