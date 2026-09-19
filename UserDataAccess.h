#pragma once
#include "RichString.h"
#include "User.h" 
#include <fstream>
#include <unordered_map>

class UserDataAccess
{
public:
	enum class enDataStatus { eSuccess = 0, eFileNotFound = 1, eFileEmpty = 2 };

private:

    static void AddDataLineToFile(string fileName, string dataLine)
    {
        fstream file;
        file.open(fileName, ios::out | ios::app);

        if (file.is_open())
        {
            file << dataLine << endl;
            file.close();
        }
    }



};

