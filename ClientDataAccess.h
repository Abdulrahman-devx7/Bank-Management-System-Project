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

    static Client ConvertClientLineToRecord(string Line, string delimiter = "#//#")
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

    static string ConvertRecordToLine(Client client, string delimiter = "#//#")
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

    static enDataStatus LoadData(string fileName, unordered_map<string, Client>& clients, string delimiter = "#//#")
    {
        fstream clientFile;
        clientFile.open(fileName, ios::in);

        if (clientFile.is_open())
        {
            string line = "";
            Client client = Client::getEmptyClient();

            while (getline(clientFile, line))
            {
                client = ConvertClientLineToRecord(line, delimiter);

                if (!client.IsMarkedForDeletion())
                    clients.insert({ client.account().AccountNumber(), client });
            }
            clientFile.close();
            return enDataStatus::eSuccess;
        }
        else
            return enDataStatus::eFileNotFound;
    }

    static void SaveData(string fileName, unordered_map<string, Client>& clients)
    {
        fstream clientFile;
        clientFile.open(fileName, ios::out);

        if (clientFile.is_open())
        {
            for (auto& [accountNumber, client] : clients)
            {
                string line = ConvertRecordToLine(client);

                clientFile << line << "\n";
            }
            clientFile.close();
        }
    }

};

