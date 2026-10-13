#pragma once
#include "RichString.h"
#include "Client.h"
#include <fstream>
#include <unordered_map>

class ClientDataAccess
{
public:
    enum class enDataStatus { eSuccess = 0, eFileNotFound = 1 };

private:

    static void AddDataLineToFile(const string& fileName, const string& dataLine)
    {
        bool needsLeadingNewline = false;
        {
            ifstream checkFile(fileName, ios::binary | ios::ate);
            if (checkFile.is_open() && checkFile.tellg() > 0)
            {
                checkFile.seekg(-1, ios::end);
                char lastChar = 0;
                checkFile.get(lastChar);
                if (lastChar != '\n' && lastChar != '\r')
                    needsLeadingNewline = true;
            }
        }

        fstream file;
        file.open(fileName, ios::out | ios::app);

        if (file.is_open())
        {
            if (needsLeadingNewline)
                file << "\n";
            file << dataLine << endl;
            file.close();
        }
    }

    static Client ConvertClientLineToRecord(const string& Line, const string& delimiter = "#//#")
    {
        vector<string> ClientData = RichString::Split(Line, delimiter);

        if (ClientData.size() != 7)
            return Client::getEmptyClient();

        Client parsedClient(Client::Mode::Update, ClientData[0], ClientData[1], ClientData[2],
            ClientData[3], ClientData[4], stod(ClientData[5]));

        if (ClientData[6] == "1") 
            parsedClient.MarkForDeletion();

        return parsedClient;
    }

    static string ConvertRecordToLine(const Client& client, const string& delimiter = "#//#")
    {
        string recordLine = "";

        recordLine += client.FullName() + delimiter;
        recordLine += client.Email() + delimiter;
        recordLine += client.PhoneNumber() + delimiter;

        recordLine += client.account().AccountNumber() + delimiter;
        recordLine += client.account().PIN() + delimiter;
        recordLine += to_string(client.account().Balance()) + delimiter;
        recordLine += (client.IsMarkedForDeletion() ? "1" : "0");

        return recordLine;
    }

public:

    static void AppendUserRecord(const Client& target, const string& filename, const string& delimiter = "#//#")
    {
        AddDataLineToFile(filename, ConvertRecordToLine(target, delimiter));
    }


    static enDataStatus LoadData(const string& fileName, unordered_map<string, Client>& clients, const string& delimiter = "#//#")
    {
        fstream clientFile;
        clientFile.open(fileName, ios::in);

        if (clientFile.is_open())
        {
            string line = "";
            Client client = Client::getEmptyClient();

            while (getline(clientFile, line))
            {
                if (line.empty())
                    continue;

                client = ConvertClientLineToRecord(line, delimiter);

                if (!client.IsEmpty() && !client.IsMarkedForDeletion())
                    clients.insert({ client.account().AccountNumber(), client });
            }
            clientFile.close();
            return enDataStatus::eSuccess;
        }
        else
            return enDataStatus::eFileNotFound;
    }

    static void SaveData(const string& fileName, const unordered_map<string, Client>& clients)
    {
        fstream clientFile;
        clientFile.open(fileName, ios::out);

        if (clientFile.is_open())
        {
            for (const auto& [accountNumber, client] : clients)
            {
                string line = ConvertRecordToLine(client);

                clientFile << line << "\n";
            }
            clientFile.close();
        }
    }

};

