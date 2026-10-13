#pragma once
#include <iostream>
#include <string>
#include <iomanip>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "User.h"

using namespace std;

class IScreen
{
protected:

    inline static const std::string UI_LINE_BOUNDS = std::string(44, '=');    
    inline static const int HEADER_BOX_WIDTH = 45;
    inline static const int SCREEN_WIDTH = 118;
    inline static const int SCREEN_INDENT = 25;

    static int GetConsoleWidth()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hOut, &csbi))
        {
            int windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            if (windowWidth > 0)
                return windowWidth;
            if (csbi.dwSize.X > 0)
                return csbi.dwSize.X;
        }
#endif
        return 120;
    }

    static int GetCenterIndent(int contentWidth)
    {
        int consoleWidth = GetConsoleWidth();
        if (consoleWidth > contentWidth)
            return (consoleWidth - contentWidth) / 2;
        return 0;
    }

    static string CenterText(const string& text, int width)
    {
        if ((int)text.length() >= width)
            return text;
        int totalPadding = width - (int)text.length();
        int leftPadding = totalPadding / 2;
        int rightPadding = totalPadding - leftPadding;
        return string(leftPadding, ' ') + text + string(rightPadding, ' ');
    }

    static string FormatCell(const string& text, size_t width)
    {
        if (text.length() > width)
        {
            if (width > 3)
                return text.substr(0, width - 2) + "..";
            return text.substr(0, width);
        }
        return text;
    }

    static bool CheckAccessRights(const User &activeUser, const User::Permissions screenPermission)
    {
        if (activeUser.CheckAccessPermission(screenPermission))
            return true;

        else
        {
            PrintScreenHeader("ACCESS DENIED", "You currently have no access\n to this page.\nPlease, contact your admin.");
            return false;
        }
    }

public:
    static void PrintScreenHeader(const string& ScreenTitle, const string& SubTitle = "")
    {
        int indent = GetCenterIndent(HEADER_BOX_WIDTH);
        string pad(indent, ' ');

        cout << pad << string(HEADER_BOX_WIDTH, '-') << "\n";
        cout << pad << CenterText(ScreenTitle, HEADER_BOX_WIDTH) << "\n";
        cout << pad << string(HEADER_BOX_WIDTH, '-') << "\n\n";

        if (!SubTitle.empty())
        {
            cout << pad << string(HEADER_BOX_WIDTH, '-') << "\n";
            size_t start = 0;
            while (start < SubTitle.length())
            {
                size_t end = SubTitle.find('\n', start);
                string line = (end == string::npos) ? SubTitle.substr(start) : SubTitle.substr(start, end - start);
                cout << pad << CenterText(line, HEADER_BOX_WIDTH) << "\n";
                if (end == string::npos)
                    break;
                start = end + 1;
            }
            cout << pad << string(HEADER_BOX_WIDTH, '-') << "\n\n";
        }
    }

    static string GetIndentPadding(int contentWidth = HEADER_BOX_WIDTH)
    {
        return string(GetCenterIndent(contentWidth), ' ');
    }

    static void PromptUser(const string& message)
    {
        cout << "\n\n" << GetIndentPadding() << message;
        system("pause>0");
    }

    static void ResetScreen()
    {
        system("cls");
        system("color 0f");
    }

    virtual void Execute() = 0;   
    virtual ~IScreen() = default;

};

