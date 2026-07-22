#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

enum class enMenuChoice {ShowClients=1, AddClient=2, DeleteClient=3, UpdateClient=4, FindClient=5, Exit=6};

const string CLIENTS_FILE_NAME = "clients.txt";
const string UI_LINE_BOUNDS(44, '=');
const int SCREEN_WIDTH = 125;

struct stNumericInputData
{
    string inputMessage;
    int from = INT_MIN;
    int to = INT_MAX;
    string validationErrorMessage = "Please, enter a valid input!\n";
};

struct stClientData
{
    string accountNumber = "";
    string user_name = "";
    string phoneNumber = "";
    string PIN_Number = "";

    long long balanceEGP = 0;
    bool MarkForDelete = false;
};

void PromptUserToGetMenu()
{
    cout << "\n\nPress any button to get back to the main menu...";
    system("pause>0");

    //ShowMainMenu()
}

vector<string> SplitString(string& S1, string delimiter = "#//#")
{
    vector<string> Words;
    int pos = 0;
    string sWord;

    while ((pos = S1.find(delimiter)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            Words.push_back(sWord);
        }
        S1.erase(0, pos + delimiter.length());
    }

    if (S1 != "")
    {
        Words.push_back(S1);
    }

    return Words;
}

stClientData ConvertLineToRecord(string Line, string delimiter = "#//#")
{
    stClientData client;
    vector<string> vClientData = SplitString(Line, delimiter);

    if (vClientData.size() >= 5)
    {
        client.accountNumber = vClientData[0];
        client.PIN_Number = vClientData[1];
        client.user_name = vClientData[2];
        client.phoneNumber = vClientData[3];
        client.balanceEGP = stold(vClientData[4]);
    }

    return client;
}

vector<stClientData> LoadFileContentsToVector(string fileName, string delimiter)
{
    vector<stClientData> clients;
    fstream file;

    file.open(fileName, ios::in);

    if (file.is_open())
    {
        string line = "";
        stClientData client;

        while (getline(file, line))
        {
            client = ConvertLineToRecord(line, delimiter);
            clients.push_back(client);
        }
        file.close();
    }

    return clients;
}

int ReadNumber(const stNumericInputData& input)
{
    int Number = 0;
    cout << input.inputMessage << endl;
    cin >> Number;

    while (cin.fail() || Number < input.from || Number > input.to
        || (std::cin.peek() != '\n' && std::cin.peek() != EOF))
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << input.validationErrorMessage;

        cin >> Number;
    }
    return Number;
}

string readAccountNumber()
{
    cout << "Please enter the account number: ";
    string userInput = "";
    cin >> userInput;

    return userInput;
}

void PrintScreenHeader(string ScreenTitle)
{
    cout << "---------------------------------------------\n";
    cout << right << setw(30) << ScreenTitle << "\n";
    cout << "---------------------------------------------\n\n";
}

char DetermineAgain(string message)
{
    char PlayAgain;
    cout << message;
    cin >> PlayAgain;

    while (cin.fail() || (toupper(PlayAgain) != 'N' && toupper(PlayAgain) != 'Y')
        || (std::cin.peek() != '\n' && std::cin.peek() != EOF))
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Please, enter a valid Choice (Y/N)!\n";

        cin >> PlayAgain;
    }
    cin.ignore();

    return PlayAgain;
}

bool FindClientByAccountNumber(const string& userInputAccNumber, stClientData& client)
{
    vector<stClientData> clients = LoadFileContentsToVector(CLIENTS_FILE_NAME, "#//#");

    for (stClientData C : clients)
    {
        if (C.accountNumber == userInputAccNumber)
        {
            client = C;
            return true;
        }
    }
    return false;
}

void MarkClientForDeleteByAccountNumber(const int indexNumber,  vector<stClientData>& clients)
{
    clients[indexNumber].MarkForDelete = true;
}

bool CheckClientExistByAccNumber(string accountNumber, vector <stClientData>& clients)
{
    for(stClientData &client : clients)
    {
        if (client.accountNumber == accountNumber)
            return true;
    }
    return false;
}

void readClientDataUpdates(stClientData& data)
{
    cout << "\nEnter PIN code: ";
    getline(cin >> ws, data.PIN_Number);

    cout << "\nEnter name: ";
    getline(cin, data.user_name);

    cout << "\nEnter phone number: ";
    getline(cin, data.phoneNumber);

    cout << "\nEnter account balance: ";
    cin >> data.balanceEGP;
}

void readClientData(stClientData& data, vector <stClientData> &clients)
{
    cout << "Enter account number: ";
    bool isExistent = false;
    do
    {
        getline(cin >> ws, data.accountNumber);
        isExistent = CheckClientExistByAccNumber(data.accountNumber, clients);

        if (isExistent)
            cout << "\nThe client with the account number[" << data.accountNumber << "] already exists, enter a different account number: ";

    } while (isExistent);

    cout << "\nEnter PIN code: ";
    getline(cin, data.PIN_Number);

    cout << "\nEnter name: ";
    getline(cin, data.user_name);

    cout << "\nEnter phone number: ";
    getline(cin, data.phoneNumber);

    cout << "\nEnter account balance: ";
    cin >> data.balanceEGP;
}

string ConvertRecordToLine(const stClientData& data, string delimiter = "#//#")
{
    string recordLine = "";

    recordLine += data.accountNumber + delimiter;
    recordLine += data.PIN_Number + delimiter;
    recordLine += data.user_name + delimiter;
    recordLine += data.phoneNumber + delimiter;
    recordLine += to_string(data.balanceEGP);

    return recordLine;
}

void AddDataLineToFile(string fileName, string dataLine)
{
    fstream file;
    file.open(fileName, ios::out | ios::app);

    if (file.is_open())
    {
        file << dataLine << endl;
        file.close();
    }
}

void AddNewClient(string fileName, vector<stClientData> &clients)
{
    stClientData userData;
    readClientData(userData, clients);
    AddDataLineToFile(fileName, ConvertRecordToLine(userData));
}

void ResetScreen()
{
    system("cls");
    system("color 0f");
}

void ShowMainMenu()
{
    cout << UI_LINE_BOUNDS << "\n";
    cout << right << setw(22) << "MAIN MENU" << "\n";
    cout << UI_LINE_BOUNDS << "\n";

    cout << right << setw(10) << "[1]" << " Show Client List" << "\n";
    cout << right << setw(10) << "[2]" << " Add New Client" << "\n";
    cout << right << setw(10) << "[3]" << " Delete Client" << "\n";
    cout << right << setw(10) << "[4]" << " Update Client Info" << "\n";
    cout << right << setw(10) << "[5]" << " Find Client" << "\n";
    cout << right << setw(10) << "[6]" << " Exit" << "\n";
   
    cout << UI_LINE_BOUNDS << "\n";
}

void PrintFileInfoHeader(const vector<stClientData>& clients)
{
    cout << right << setw(62) << "Client list: (" << clients.size() << ") client(s)" << "\n";
    cout << string(SCREEN_WIDTH, '-') << "\n";
    cout << left << "| " << setw(20) << " Account Number " << "| ";
    cout << setw(15) << " PIN Number " << "| ";
    cout << setw(50) << " Client Name " << "| ";
    cout << setw(15) << "Phone Number" << "| ";
    cout << setw(14) << " Balance " << "|" << "\n";
    cout << string(SCREEN_WIDTH, '-') << "\n";
}

void PrintClientRecord(const stClientData& data)
{
    cout << UI_LINE_BOUNDS;

    cout << "\n" << left << setw(20) << "Account Number: " << data.accountNumber << "\n";
    cout << left << setw(20) << "PIN code: " << data.PIN_Number << "\n";
    cout << left << setw(20) << "Name: " << data.user_name << "\n";
    cout << left << setw(20) << "Phone Number: " << data.phoneNumber << "\n";
    cout << left << setw(20) << "Account Balance: " << data.balanceEGP << "\n";
    
    cout << UI_LINE_BOUNDS;
}

void EvaluateMenuChoice(enMenuChoice &menuChoice)
{
    stNumericInputData inputData;
    inputData.inputMessage = "Choose the operation you'd like to do (1-6)\n";
    inputData.from = 1;
    inputData.to = 6;
    inputData.validationErrorMessage = "\nPlease, enter a number in a valid range from the menu (1-6)!\n";

    menuChoice = enMenuChoice(ReadNumber(inputData));
}

void PrintIndividualUserInfo(const stClientData& client)
{
    cout << "| " << setw(20) << left << client.accountNumber << "| ";
    cout << setw(15) << left << client.PIN_Number << "| ";
    cout << setw(50) << left << client.user_name << "| ";
    cout << setw(15) << left << client.phoneNumber << "| ";
    cout << setw(14) << left << client.balanceEGP << "| " << "\n";
}

void SaveVectorContentsToFile(string fileName, const vector<stClientData>& clients)
{
    fstream file;
    file.open(fileName, ios::out);

    if (file.is_open())
    {
        for (const stClientData& client : clients)
        {
            string line = ConvertRecordToLine(client);

            file << line << "\n";
        }
        file.close();
    }
}

void ShowClientListScreen(string fileName)
{
    PrintScreenHeader("SHOW CLIENT LIST");
    vector<stClientData> clients = LoadFileContentsToVector(fileName, "#//#");

    PrintFileInfoHeader(clients);

    if (clients.size() == 0)
        cout << right << setw(62) << "NO CLIENTS ARE AVAILABLE IN THE SYSTEM!";

    for (stClientData& client : clients)
    {
        if (client.MarkForDelete == false)
            PrintIndividualUserInfo(client);
    }
    cout << string(SCREEN_WIDTH, '-') << "\n";
}

void UpdateClientData(vector<stClientData>& clients, int indexNumber)
{
    stClientData client;

    // The optionality of updating individual fields in the client info should be added instead of
    // prompting the user to type each field again. This would require him to retype data it doesn't want to update
    readClientDataUpdates(client);
    clients[indexNumber] = client;
}

void UpdateClientByAccountNumber(vector<stClientData>& clients, string fileName)
{
    stClientData Client;
    string accountNumber = readAccountNumber();

    if (FindClientByAccountNumber(accountNumber, Client))
    {
        PrintClientRecord(Client);
        if (toupper(DetermineAgain("\nDo you want to update this client's data (Y/N)?\n")) == 'Y')
        {
            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].accountNumber == accountNumber)
                {
                    UpdateClientData(clients, i);
                    break;
                }
            }
            SaveVectorContentsToFile(fileName, clients);
            cout << "\nClient data updated successfully\n ";
        }
    }
    else
        cout << "\nClient with account number (" << accountNumber << ") is not found!\n";
}

void DeleteClientByAccountNumber(vector<stClientData>& clients, string fileName)
{
    stClientData Client;
    string accountNumber = readAccountNumber();

    if (FindClientByAccountNumber(accountNumber, Client))
    {
        PrintClientRecord(Client);
        if (toupper(DetermineAgain("Are you sure you want to delete this client (Y/N)?\n")) == 'Y')
        {
            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].accountNumber == accountNumber)
                {
                    MarkClientForDeleteByAccountNumber(i, clients);
                    clients.erase(clients.begin() + i);
                    break;
                }
            }
            SaveVectorContentsToFile(fileName, clients);
            cout << "\nClient deleted successfully\n ";
        }
    }
    else
        cout << "\nClient with account number (" << accountNumber << ") is not found!\n";
}

void AddClientsScreen(string fileName)
{
    PrintScreenHeader("ADD NEW CLIENT");

    vector<stClientData> clients = LoadFileContentsToVector(fileName, "#//#");
    do
    {
        AddNewClient(fileName, clients);
        cout << "Client added successfully, ";

    } while (toupper(DetermineAgain("do you want to add more clients (Y/N)? \n")) == 'Y');

    SaveVectorContentsToFile(fileName, clients);
}

void DeleteClientScreen(string fileName)
{
    PrintScreenHeader("DELETE CLIENT");
    vector<stClientData> clients = LoadFileContentsToVector(fileName, "#//#");
    DeleteClientByAccountNumber(clients, fileName);
}

void UpdateClientScreen(string fileName)
{
    PrintScreenHeader("UPDATE CLIENT INFO");
    vector<stClientData> clients = LoadFileContentsToVector(fileName, "#//#");
    UpdateClientByAccountNumber(clients, fileName);
}

void FindClientByAccountNumber(vector<stClientData>& clients)
{
    string userInput = readAccountNumber();

    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i].accountNumber == userInput)
        {
            cout << "\n";
            stClientData client = clients[i];

            PrintClientRecord(client);

            return;
        }
    }

    cout << "\nClient with the account number: " << userInput << " is not found!";
}

void FindClientScreen(string fileName)
{
    PrintScreenHeader("FIND CLIENT");
    vector<stClientData> clients = LoadFileContentsToVector(fileName, "#//#");
    FindClientByAccountNumber(clients);
}


void ShowDeleteClientScreen()
{
    PrintScreenHeader("DELETE CLIENT");
    vector<stClientData> clients = LoadFileContentsToVector(CLIENTS_FILE_NAME, "#//#");
    DeleteClientByAccountNumber(clients, CLIENTS_FILE_NAME);
}

void RunRestOfMenuOptions(const enMenuChoice choice)
{
    //SOME OF THE CRUD OPERATIONS WORK BY ASKING FOR THE ACCOUNT NUMBER
    switch (choice)
    {
    case enMenuChoice::ShowClients:
        ResetScreen();
        ShowClientListScreen(CLIENTS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enMenuChoice::AddClient:
        ResetScreen();
        AddClientsScreen(CLIENTS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enMenuChoice::DeleteClient:
        ResetScreen();
        ShowDeleteClientScreen();
        PromptUserToGetMenu();
        break;

    case enMenuChoice::UpdateClient:
        ResetScreen();
        UpdateClientScreen(CLIENTS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enMenuChoice::FindClient:
        ResetScreen();
        FindClientScreen(CLIENTS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enMenuChoice::Exit:
        break;
    }
}

void StartBankSystem()
{
    enMenuChoice RunningState = enMenuChoice::Exit;
    do
    {
        ResetScreen();
        ShowMainMenu();

        EvaluateMenuChoice(RunningState);

        if (RunningState != enMenuChoice::Exit)
            RunRestOfMenuOptions(RunningState);

    } while (RunningState != enMenuChoice::Exit);
}

int main()
{
    StartBankSystem();
}
