#pragma once
#include "IScreen.h"
#include "TransferRecordStruct.h"
#include "User.h"
#include <vector>
#include <iomanip>
#include <iostream>

class MoneyTransferScreen : public IScreen
{
private:
    vector<TransferRecordStruct>& _transferRegisters;
    User& _activeUser;

    static const int SCREEN_WIDTH = 150;
    static const int SCREEN_INDENT = 15;

    static void PrintFileInfoHeader(const vector<TransferRecordStruct>& registers)
    {
        std::string indent(SCREEN_INDENT, ' ');

        std::cout << "\n" << indent
            << std::setw((SCREEN_WIDTH + 28) / 2) << std::right
            << "Money Transfer Register List: (" + std::to_string(registers.size()) + ") Record(s)\n";

        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";

        std::cout << indent << "| " << std::left
            << std::setw(12) << "Date" << " | "
            << std::setw(10) << "Time" << " | "
            << std::setw(14) << "Sender Acc" << " | "
            << std::setw(14) << "Receiver Acc" << " | "
            << std::setw(12) << "Amount" << " | "
            << std::setw(14) << "Sender Bal" << " | "
            << std::setw(14) << "Receiver Bal" << " | "
            << std::setw(14) << "Executor" << " |" << "\n";

        std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";
    }

    static void PrintIndividualTableInfo(const TransferRecordStruct& record)
    {
        const std::string indent(SCREEN_INDENT, ' ');

        std::cout << indent << "| " << std::left
            << std::setw(12) << Date::DateToString(record.transferDate) << " | "
            << std::setw(10) << Time::TimeToString(record.transferTime) << " | "
            << std::setw(14) << record.senderAccountNumber << " | "
            << std::setw(14) << record.receiverAccountNumber << " | "
            << std::setw(12) << std::fixed << std::setprecision(2) << record.transferredAmount << " | "
            << std::setw(14) << std::fixed << std::setprecision(2) << record.senderBalance << " | "
            << std::setw(14) << std::fixed << std::setprecision(2) << record.receiverBalance << " | "
            << std::setw(14) << record.transferExcecutor << " |" << "\n";
    }

    void PrintTable()
    {
        PrintFileInfoHeader(_transferRegisters);

        if (_transferRegisters.empty())
        {
            const std::string indent(SCREEN_INDENT, ' ');
            std::cout << indent << "| " << std::left << std::setw(SCREEN_WIDTH - 4) << "No transfer records found." << " |" << "\n";
            std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";
        }
        else
        {
            for (const auto& record : _transferRegisters)
            {
                PrintIndividualTableInfo(record);
            }
            const std::string indent(SCREEN_INDENT, ' ');
            std::cout << indent << std::string(SCREEN_WIDTH, '-') << "\n";
        }
    }

public:
    MoneyTransferScreen(vector<TransferRecordStruct>& transferRegisters, User& activeUser)
        : _transferRegisters(transferRegisters), _activeUser(activeUser)
    {
    }

    void Execute() override
    {
        if (!CheckAccessRights(_activeUser, User::Permissions::Transactions))
        {
            return;
        }

        ResetScreen();
        IScreen::PrintScreenHeader("TRANSFER REGISTER SCREEN");
        PrintTable();
    }
};

