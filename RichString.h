#pragma once
#include <iostream>
#include <algorithm>
#include <cctype>
#include <vector>

using namespace std;

class RichString
{
private:
    string _Value;

    static int CountByCondition(const string& text, int(*condition)(int) = [](int) { return 1; })
    {
        size_t count = 0;
        for (char letter : text)
        {
            if (condition(letter)) count++;
        }
        return count;
    }

    static string ModifyFirstLetters(string text, int (*modifier)(int), const string &delimiter = " ")
    {
        size_t start = 0;
        size_t end = text.find(delimiter);

        while (end != string::npos)
        {
            if (start != end)
                text[start] = modifier(text[start]);

            start = end + delimiter.length();
            end = text.find(delimiter, start);
        }

        if (start < text.length())
            text[start] = modifier(text[start]);

        return text;
    }

public:

    RichString()
    {
        _Value = "";
    }

    RichString(string Value)
    {
        _Value = Value;
    }

    void SetValue(string Value) {
        _Value = Value;
    }

    string Value() {
        return _Value;
    }

    // We need to add an option in which the developer can either use the static method 
    // that returns the given data normally preserving his data
    // and another option that provides manipulation in place (by ref)

    static size_t Length(const string& text)
    {
        return text.length();
    }

    size_t Length()
    {
        return Value().length();
    }

    static bool IsVowel(char Ch1)
    {
        Ch1 = tolower(Ch1);

        return ((Ch1 == 'a') || (Ch1 == 'e') || (Ch1 == 'i') || (Ch1 == 'o') || (Ch1 == 'u'));
    }

    bool isVowel(size_t index)
    {
        if ((index < 0) || (index > _Value.length()-1))
            return false;
        else
            return IsVowel(_Value[index]);
    }

    static string TrimLeft(string S1)
    {
        for (size_t i = 0; i < S1.length(); i++)
        {
            if (S1[i] != ' ')
            {
                return S1.substr(i, S1.length() - i);
            }
        }

        return "";
    }

    void TrimLeft()
    {
        _Value = TrimLeft(Value());
    }

    static string TrimRight(string S1)
    {
        for (int i = S1.length() - 1; i >= 0; i--)
        {
            if (S1[i] != ' ')
            {
                return S1.substr(0, i + 1);
            }
        }

        return "";
    }

    void TrimRight()
    {
        _Value = TrimRight(Value());
    }

    static string Trim(string S1)
    {
        return TrimLeft(TrimRight(S1));
    }

    void Trim()
    {
        _Value = Trim(Value());
    }

    static size_t CountWords(const string &text, const string& delimiter)
    {
        size_t wordCounter = 0;

        size_t start = 0;
        size_t end = text.find(delimiter);

        while (end != string::npos)
        {
            if (start != end)
                wordCounter++;

            start = end + delimiter.length();
            end = text.find(delimiter, start);
        }

        if (start < text.length())
            wordCounter++;
      
        return wordCounter;
    }

    size_t CountWords(const string &delimiter = " ")
    {
        return CountWords(_Value, delimiter);
    };

    static void PrintFirstLetters(const string &text, const string &delimiter, bool newline)
    {
        size_t start = 0;
        size_t end = text.find(delimiter);

        while (end != string::npos)
        {
            if (start != end)
            {
                if (newline)
                    cout << text[start] << endl;
                else cout << " " << text[start];
            }

            start = end + delimiter.length();
            end = text.find(delimiter, start);
        }

        if (start < text.length())
        {
            if (newline)
                cout << text[start] << endl;
            else cout << " " << text[start];
        }
    }

    void PrintFirstLetters(const string &delimiter = " ", bool newline = false)
    {
        PrintFirstLetters(Value(), delimiter, newline);
    }

    static string StartWordsWithUppercase(string text, const string& delimiter)
    {
        return ModifyFirstLetters(text, ::toupper, delimiter);
    }

    void StartWordsWithUppercase(string delimiter = " ")
    {
        _Value = StartWordsWithUppercase(Value(), delimiter);
    }

    static string StartWordsWithLowerCase(string text, const string& delimiter)
    {
        return ModifyFirstLetters(text, ::tolower, delimiter);
    }

    void StartWordsWithLowerCase(const string &delimiter = " ")
    {
        _Value = StartWordsWithLowerCase(Value(), delimiter);
    }

    static string CapitalizeString(string text)
    {
        std::transform(text.begin(), text.end(), text.begin(), ::toupper);
        return text;
    }

    void CapitalizeString()
    {
        _Value = CapitalizeString(Value());
    }

    static string DecapitalizeString(string text)
    {
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);
        return text;
    }

    void DecapitalizeString()
    {
        _Value = DecapitalizeString(Value());
    }

    static char InvertCharacter(char character)
    {
        character = isupper(character) ? tolower(character) : toupper(character);
        return character;
    }
    
    // This one might need exception handling or something to prompt the compiler for an intellisense error that this is out of bounds
    // But we'll just make it simple since <algorithm> functions don't have that. It's probably not worth...
    // No, the way out of bounds errors are handled need exception handling!!

    void InvertCharacter(int stringIndex)
    {
        if ((stringIndex < 0) || (stringIndex > _Value.length()-1))
            return;

        else
            _Value[stringIndex] = InvertCharacter(_Value[stringIndex]);
    }

    static string InvertString(string text)
    {
        for (int i = 0; i < text.length(); i++)
        {
            text[i] = InvertCharacter(text[i]);
        }
        return text;
    }

    void InvertString()
    {
        _Value = InvertString(Value());
    }

    size_t CountAllLetters()
    {
        return CountByCondition(Value(), ::isalpha);
    }

    static size_t CountAllLetters(const string& text)
    {
        return CountByCondition(text, ::isalpha);
    }

    size_t CountUppercaseLetters()
    {
        return CountByCondition(Value(), ::isupper);
    }

    static size_t CountUppercaseLetters(const string& text)
    {
        return CountByCondition(text, ::isupper);
    }

    size_t CountLowercaseLetters()
    {
        return CountByCondition(Value(), ::islower);
    }

    static size_t CountLowercaseLetters(const string& text)
    {
        return CountByCondition(text, ::islower);
    }

    size_t CountPunctuation()
    {
        return CountByCondition(Value(), ::ispunct);
    }

    static size_t CountPunctuation(const string& text)
    {
        return CountByCondition(text, ::ispunct);
    }

    size_t CountDigits()
    {
        return CountByCondition(Value(), ::isdigit);
    }

    static size_t CountDigits(const string& text)
    {
        return CountByCondition(text, ::isdigit);
    }

    size_t CountAlphanumeric()
    {
        return CountByCondition(Value(), ::isalnum);
    }

    static size_t CountAlphanumeric(const string& text)
    {
        return CountByCondition(text, ::isalnum);
    }

    size_t CountHexDigits()
    {
        return CountByCondition(Value(), ::isxdigit);
    }

    static size_t CountHexDigits(const string& text)
    {
        return CountByCondition(text, ::isxdigit);
    }

    size_t CountSpaces()
    {
        return CountByCondition(Value(), ::isspace);
    }

    static size_t CountSpaces(const string& text)
    {
        return CountByCondition(text, ::isspace);
    }

    static size_t CountSpecificLetter(const string &text, char letter, bool matchCase = true)
    {
        size_t Counter = 0;

        for (int i = 0; i < text.length(); i++)
        {
            if (matchCase)
            {
                if (text[i] == letter)
                    Counter++;
            }
            else
            {
                if (tolower(text[i]) == tolower(letter))
                    Counter++;
            }
        }
        return Counter;
    }

    size_t CountSpecificLetter(char letter, bool matchCase = true)
    {
        return CountSpecificLetter(Value(), letter, matchCase);
    }

    static size_t CountVowels(const string &text)
    {
        size_t vowelCounter = 0;
        for (size_t i = 0; i < text.size(); i++)
        {
            if (IsVowel(text[i]))
                vowelCounter++;
        }
        return vowelCounter;
    }

    size_t CountVowels()
    {
        return CountVowels(Value());
    }

    static void PrintEachWordInText(const string &text, const string& delimiter, bool newline)
    {
        size_t start = 0;
        size_t end = text.find(delimiter);

        while (end != string::npos)
        {
            if (start != end)
            {
                if (newline)
                    cout << Trim(text.substr(start, end - start)) << endl;
                else
                    cout << Trim(text.substr(start, end - start)) << " ";
            }

            start = end + delimiter.length();
            end = text.find(delimiter, start);
        }

        if (start < text.length())
        {
            if (newline)
                cout << Trim(text.substr(start, end - start)) << endl;
            else
                cout << Trim(text.substr(start, end - start)) << " ";
        }
    }

    void PrintEachWordInText(string delimiter = " ", bool newline = false)
    {
        PrintEachWordInText(Value(), delimiter, newline);
    }

    static vector<string> Split(const string& text, const string& delimiter)
    {
        vector<string> tokens;
        if (text.empty()) return tokens;

        size_t start = 0;
        size_t end = text.find(delimiter);

        while (end != string::npos)
        {
            if (start != end)
            {
                tokens.push_back(Trim(text.substr(start, end - start)));
            }

            start = end + delimiter.length();
            end = text.find(delimiter, start);
        }

        if (start < text.length())
        {
            tokens.push_back(Trim(text.substr(start)));
        }

        return tokens;
    }

    vector<string> Split(const string& delimiter = " ")
    {
        return Split(Value(), delimiter);
    }

    static string JoinString(const vector<string>& words, const string& delimiter)
    {
        string text;

        for (int i = 0; i < words.size(); i++)
        {
            text += words[i];

            if (i != words.size() - 1)
                text.append(delimiter);
        }
        return text;
    }

    static string ReverseWords(const string &text, const string &delimiter)
    {
        vector<string> TextElements = Split(text, delimiter);

        string reversedText = "";

        for (int i = TextElements.size() - 1; i >= 0; i--)
        {
            reversedText += TextElements[i];

            if (i != 0)
                reversedText+=delimiter;
        }
        return reversedText;
    }

    void ReverseWords(string const& delimiter = " ")
    {
        _Value = ReverseWords(Value(), delimiter);
    }

    static string ReplaceWord(const string& text, const string& targetWord, const string& replacementString, const string& delimiter)
    {
        vector<string> textElements = Split(text, delimiter);
        string updatedText = "";

        // Guard Clause, Abort the function immediately to avoid useless work
        if (textElements.empty())
            return "";

        auto it = textElements.begin();

        while (it != textElements.end())
        {
            if (*it == targetWord)
            {
                *it = replacementString;
            }

            updatedText += *it;

            if (it != textElements.end() - 1)
                updatedText += delimiter;

            ++it;
        }

        return updatedText;
    }

    void ReplaceWord(const string& targetWord, const string& replacementString, const string &delimiter = " ")
    {
        _Value = ReplaceWord(Value(), targetWord, replacementString, delimiter);
    }

    static string Replace(string text, const string& target, const string& replacement)
    {
        if (target.empty()) return text;

        size_t pos = 0;

        while ((pos = text.find(target, pos)) != string::npos)
        {
            text.replace(pos, target.length(), replacement);

            pos += replacement.length();
        }

        return text;
    }

    void Replace(const string& target, const string& replacement)
    {
        _Value = Replace(Value(), target, replacement);
    }

    static string RemovePunctuations(const string& text)
    {
        string updatedString = "";
        for (int i = 0; i < text.length(); i++)
        {
            if (!ispunct(text[i]))
                updatedString.push_back(text[i]);
        }
        return updatedString;
    }

    void RemovePunctuations()
    {
        _Value = RemovePunctuations(Value());
    }

};

