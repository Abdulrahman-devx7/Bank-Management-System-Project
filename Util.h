#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <utility>

using namespace std;

class Util
{
public:
    enum class enCharType { SmallLetter = 1, CaptialLetter = 2, SpecialCharacter = 3, NumericalDigit = 4, Random = 5 };

    static void RandomSeeder()
    {
        srand((unsigned)time(NULL));
    }

    static int RandomNumber(int From, int To)
    {
        int randNum = rand() % (To - From + 1) + From;
        return randNum;
    }

    static char GetRandomCharacter(enCharType type)
    {
        if (type == enCharType::Random)
            type = static_cast<enCharType>(RandomNumber(1, 5));

        switch (type)
        {
        case enCharType::SmallLetter:
            return char(RandomNumber(97, 122));

        case enCharType::CaptialLetter:
            return char(RandomNumber(65, 90));

        case enCharType::SpecialCharacter:
        {
            short SequenceToPickFromASCII = RandomNumber(1, 4);

            if (SequenceToPickFromASCII == 1)
                return char(RandomNumber(33, 47));

            else if (SequenceToPickFromASCII == 2)
                return char(RandomNumber(58, 64));

            else if (SequenceToPickFromASCII == 3)
                return char(RandomNumber(91, 96));

            else
                return char(RandomNumber(123, 126));
        }

        case enCharType::NumericalDigit:
            return char(RandomNumber(48, 57));

        default:
            return '\0';
        }
    }

    static std::vector<int> ShuffleArray(vector<int>& originalVector)
    {
        for (int i = 0; i < originalVector.size(); i++)
        {
            int index1 = RandomNumber(1, originalVector.size()) - 1;
            int index2 = RandomNumber(1, originalVector.size()) - 1;

            swap(originalVector[index1], originalVector[index2]);
        }
        return originalVector;
    }

    static void FillWithRandomNumbers(vector<int>& array, int startPos, int endPos)
    {
        if (((startPos < 0) && (startPos > array.size() - 1)) || ((endPos < 0) && (endPos > array.size() - 1)))
        {
            cout << "WARNING: BOUNDARY ERROR\nFILLING ABORTED\n";
            return;
        }

        for (int i = startPos; i <= startPos; i++)
        {
            int Random = RandomNumber(1, 100);
            array[i] = Random;
        }
    }

    static std::string GenerateRandomString(enCharType type, int stringLength)
    {
        std::string BuildString;
        for (int i = 0; i < stringLength; i++)
        {
            BuildString += GetRandomCharacter(type);
        }
        return BuildString;
    }

    static std::string GenerateKey(enCharType type, int chunkLength)
    {
        string Key;

        for (int i = 0; i < 4; i++)
        {
            Key += GenerateRandomString(type, chunkLength);
            if (i < 3) Key += "-";
        }
        return Key;
    }

    static void GenerateKeys(vector<string>& keyContainer, int numKeys, enCharType type, int chunkLength)
    {
        for (int i = 0; i < numKeys; i++)
            keyContainer[i] = GenerateKey(type, chunkLength);
    }

    static std::string EncryptText(std::string text, int key)
    {
        for (char& c : text)
        {
            if (isdigit(c))
                c = '0' + ((c - '0' + (key % 10)) % 10);
            else if (isupper(c))
                c = 'A' + ((c - 'A' + (key % 26)) % 26);
            else if (islower(c))
                c = 'a' + ((c - 'a' + (key % 26)) % 26);
        }
        return text;
    }

    static std::string DecryptText(std::string text, int key)
    {
        for (char& c : text)
        {
            if (isdigit(c))
                c = '0' + ((c - '0' - (key % 10) + 10) % 10);
            else if (isupper(c))
                c = 'A' + ((c - 'A' - (key % 26) + 26) % 26);
            else if (islower(c))
                c = 'a' + ((c - 'a' - (key % 26) + 26) % 26);
        }
        return text;
    }
};
