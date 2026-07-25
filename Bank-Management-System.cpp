#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>
#include <unordered_map>

using namespace std;
enum class enMenuChoice {ShowClients=1, AddClient=2, DeleteClient=3, UpdateClient=4, FindClient=5, Transactions=6, Exit=7};
enum class enTransactionsMenuChoice {Deposit=1, Withdraw=2, TotalBalances=3, MainMenu=4};

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

    long long balanceUSD = 0;
    bool MarkForDelete = false;
};

void PromptUserToGetMenu()
{
    cout << "\n\nPress any button to get back to the previous menu...";
    system("pause>0");
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
        client.balanceUSD = stold(vClientData[4]);
    }

    return client;
}

void LoadClientsFromFile(string fileName, vector<stClientData>& clients, string delimiter = "#//#")
{
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
}

void LoadClientsFromFile(string fileName, unordered_map<string, stClientData>& clients, string delimiter = "#//#")
{
    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open())
    {
        string line = "";
        stClientData client;

        while (getline(file, line))
        {
            client = ConvertLineToRecord(line, delimiter);
            clients.insert({ client.accountNumber, client });
        }
        file.close();
    }
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

auto FindClientByAccountNumber(const string& userInputAccNumber,unordered_map<string, stClientData> &clients)
{
    return clients.find(userInputAccNumber);
}

void PrintClientRecord(const stClientData& data)
{
    cout << UI_LINE_BOUNDS;

    cout << "\n" << left << setw(20) << "Account Number: " << data.accountNumber << "\n";
    cout << left << setw(20) << "PIN code: " << data.PIN_Number << "\n";
    cout << left << setw(20) << "Name: " << data.user_name << "\n";
    cout << left << setw(20) << "Phone Number: " << data.phoneNumber << "\n";
    cout << left << setw(20) << "Account Balance: " << data.balanceUSD << "\n";

    cout << UI_LINE_BOUNDS;
}

void DetermineAccountFind(unordered_map<string, stClientData>& clients)
{
    string accountNumber = readAccountNumber();

    auto clientIt = FindClientByAccountNumber(accountNumber, clients);

    if (clientIt != clients.end())
    {
        cout << "\n";
        PrintClientRecord(clientIt->second);
    }
    else 
        cout << "\nClient with the account number: " << accountNumber << " is not found!";
}

void MarkClientForDeleteByAccountNumber(unordered_map<string, stClientData> &clients, string accountNumber)
{
    clients[accountNumber].MarkForDelete = true;
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
    cin >> data.balanceUSD;
}

void readClientData(stClientData& client, vector <stClientData> &clients)
{
    cout << "Enter account number: ";
    bool isExistent = false;
    do
    {
        getline(cin >> ws, client.accountNumber);
        isExistent = CheckClientExistByAccNumber(client.accountNumber, clients);

        if (isExistent)
            cout << "\nThe client with the account number[" << client.accountNumber << "] already exists, enter a different account number: ";

    } while (isExistent);

    cout << "\nEnter PIN code: ";
    getline(cin, client.PIN_Number);

    cout << "\nEnter name: ";
    getline(cin, client.user_name);

    cout << "\nEnter phone number: ";
    getline(cin, client.phoneNumber);

    cout << "\nEnter account balance: ";
    cin >> client.balanceUSD;
}

int ReadDepositNumber()
{
    stNumericInputData inputData;
    inputData.inputMessage = "\n\nPlease, enter the amount you would like to deposit\n";
    inputData.from = 0;

    return ReadNumber(inputData);
}

int ReadWithdrawNumber()
{
    stNumericInputData inputData;
    inputData.inputMessage = "\n\nPlease, enter the amount you would like to withdraw\n";
    inputData.from = 0;

    return ReadNumber(inputData);
}

long long AccumulateBalances(const vector<stClientData>& clients)
{
    long long sumBalances = 0;

    for (int i = 0; i < clients.size(); i++)
    {
        sumBalances += clients[i].balanceUSD;
    }

    return sumBalances;
}

string ConvertRecordToLine(const stClientData& data, string delimiter = "#//#")
{
    string recordLine = "";

    recordLine += data.accountNumber + delimiter;
    recordLine += data.PIN_Number + delimiter;
    recordLine += data.user_name + delimiter;
    recordLine += data.phoneNumber + delimiter;
    recordLine += to_string(data.balanceUSD);

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

    clients.push_back(userData);
    AddDataLineToFile(fileName, ConvertRecordToLine(userData));
}

bool CheckBalanceForWithdrawal(const stClientData& client, int withdrawalAmount)
{
    if (withdrawalAmount > client.balanceUSD)
    {
        cout << "\nThe entered amount exceeds the existing balance. You can withdraw up to " << client.balanceUSD << " USD\n\n";
        return false;
    }
    else return true;
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
    cout << right << setw(10) << "[6]" << " Transactions" << "\n";
    cout << right << setw(10) << "[7]" << " Exit" << "\n";
   
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
    cout << setw(14) << " Balance - USD" << "|" << "\n";
    cout << string(SCREEN_WIDTH, '-') << "\n";
}

void PrintClientBalancesHeader(const vector<stClientData>& clients)
{
    cout << right << setw(42) << "Balances List for (" << clients.size() << ") client(s)" << "\n";
    cout << string(SCREEN_WIDTH-34, '-') << "\n";

    cout << left << "| " << setw(20) << " Account Number " << "| ";
    cout << setw(50) << " Client Name " << "|";
    cout << setw(14) << "Balance " << "|" << "\n";

    cout << string(SCREEN_WIDTH-34, '-') << "\n";
}


void EvaluateMenuChoice(enMenuChoice &menuChoice)
{
    stNumericInputData inputData;
    inputData.inputMessage = "Choose the operation you'd like to do (1-7)\n";
    inputData.from = 1;
    inputData.to = 7;
    inputData.validationErrorMessage = "\nPlease, enter a number in a valid range from the menu (1-7)!\n";

    menuChoice = enMenuChoice(ReadNumber(inputData));
}

void EvaluateMenuChoice(enTransactionsMenuChoice& menuChoice)
{
    stNumericInputData inputData;
    inputData.inputMessage = "Choose the operation you'd like to do (1-4)\n";
    inputData.from = 1;
    inputData.to = 4;
    inputData.validationErrorMessage = "\nPlease, enter a number in a valid range from the menu (1-4)!\n";

    menuChoice = enTransactionsMenuChoice(ReadNumber(inputData));
}

void PrintIndividualUserInfo(const stClientData& client)
{
    cout << "| " << setw(20) << left << client.accountNumber << "| ";
    cout << setw(15) << left << client.PIN_Number << "| ";
    cout << setw(50) << left << client.user_name << "| ";
    cout << setw(15) << left << client.phoneNumber << "| ";
    cout << setw(14) << left << client.balanceUSD << "| " << "\n";
}

void PrintUserInfoInBalancesTable(const stClientData &client)
{
    cout << "| " << setw(20) << left << client.accountNumber << "| ";
    cout << setw(50) << left << client.user_name << "| ";
    cout << setw(14) << left << client.balanceUSD << "| " << "\n";
}

//WE MIGHT NOT NEED THIS LATER ON 
void SaveToFile(string fileName, const vector<stClientData>& clients)
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

void SaveToFile(string fileName, unordered_map<string, stClientData>& clients)
{
    fstream file;
    file.open(fileName, ios::out);

    if (file.is_open())
    {
        for (auto& [accountNumber, client] : clients)
        {
            string line = ConvertRecordToLine(client);

            file << line << "\n";
        }
        file.close();
    }
}

void ShowClientListScreen(string fileName)
{
    vector<stClientData> clients;
    LoadClientsFromFile(fileName, clients);

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

void SafeDelete(unordered_map<string, stClientData>& clients, string accountNumber)
{
    int elementsRemoved = clients.erase(accountNumber);

    if(elementsRemoved>0)
        cout << "Account successfully deleted\n";
    else cout << "Error: Account not found.\n";
}

void ShowClientsBalances(string fileName)
{
    vector<stClientData> clients;
    LoadClientsFromFile(fileName, clients);

    PrintClientBalancesHeader(clients);

    if (clients.size() == 0)
        cout << right << setw(62) << "NO CLIENTS ARE AVAILABLE IN THE SYSTEM!";

    for (stClientData& client : clients)
    {
        if (client.MarkForDelete == false)
            PrintUserInfoInBalancesTable(client);
    }
    cout << string(SCREEN_WIDTH-34, '-') << "\n";
    cout << right << setw(42) << "Total Balances = " << AccumulateBalances(clients) << " USD\n";
}

void VerifyBalanceForWithdraw(unordered_map<string, stClientData>::iterator &clientIt, int withdrawAmount)
{
    if (toupper(DetermineAgain("\nAre you sure you want to perform this transaction (Y/N)? ")) == 'Y')
    {
        while (!CheckBalanceForWithdrawal(clientIt->second, withdrawAmount))
            withdrawAmount = ReadWithdrawNumber();

        clientIt->second.balanceUSD -= withdrawAmount;
        cout << "\n\nDone! Your deposit of " << withdrawAmount << " has been added successfully. Your new balance is: "
            << clientIt->second.balanceUSD << " USD\n\n";
    }
}

void VerifyDeposit(unordered_map<string, stClientData>::iterator& clientIt, int depositAmount)
{
    if (toupper(DetermineAgain("\nAre you sure you want to perform this transaction (Y/N)? ")) == 'Y')
    {
        clientIt->second.balanceUSD += depositAmount;

        cout << "\n\nDone! Your deposit of " << depositAmount << " has been added successfully. Your new balance is: "
            << clientIt->second.balanceUSD << " USD\n\n";
    }
}

void UpdateClientData(unordered_map<string, stClientData>::iterator clientIt)
{
    // The optionality of updating individual fields in the client info should be added instead of
    // prompting the user to type each field again. This would require him to retype data it doesn't want to update
    readClientDataUpdates(clientIt->second);
}

void UpdateClientByAccountNumber(unordered_map<string, stClientData> &clients, string fileName)
{
    stClientData Client;
    string accountNumber = readAccountNumber();

    auto clientIt = FindClientByAccountNumber(accountNumber, clients);

    if (clientIt!=clients.end())
    {
        PrintClientRecord(Client);
        if (toupper(DetermineAgain("\nDo you want to update this client's data (Y/N)?\n")) == 'Y')
        {
            UpdateClientData(clientIt);

            SaveToFile(fileName, clients);
            cout << "\nClient data updated successfully\n ";
        }
    }
    else
        cout << "\nClient with account number (" << accountNumber << ") is not found!\n";
}

void DeleteClientByAccountNumber(unordered_map<string, stClientData>& clients, string fileName)
{
    stClientData Client;
    string accountNumber = readAccountNumber();

    if (FindClientByAccountNumber(accountNumber, clients)!=clients.end())
    {
        PrintClientRecord(Client);
        if (toupper(DetermineAgain("Are you sure you want to delete this client (Y/N)?\n")) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(clients, accountNumber);
            SafeDelete(clients, accountNumber);

            SaveToFile(fileName, clients);
        }
    }
    else
        cout << "\nClient with account number (" << accountNumber << ") is not found!\n";
}


void AddClientsScreen(string fileName)
{
    PrintScreenHeader("ADD NEW CLIENT");

    vector<stClientData> clients;
    LoadClientsFromFile(fileName, clients);

    do
    {
        AddNewClient(fileName, clients);
        cout << "Client added successfully, ";

    } while (toupper(DetermineAgain("do you want to add more clients (Y/N)? \n")) == 'Y');
}

void DeleteClientScreen(string fileName)
{
    PrintScreenHeader("DELETE CLIENT");
    unordered_map<string, stClientData> clients;

    LoadClientsFromFile(CLIENTS_FILE_NAME, clients, "#//#");
    DeleteClientByAccountNumber(clients, fileName);
}

void UpdateClientScreen(string fileName)
{
    PrintScreenHeader("UPDATE CLIENT INFO");
    unordered_map<string, stClientData> clients;

    LoadClientsFromFile(CLIENTS_FILE_NAME, clients, "#//#");
    UpdateClientByAccountNumber(clients, fileName);
}

void FindClientScreen(string fileName)
{
    PrintScreenHeader("FIND CLIENT");
    unordered_map<string, stClientData> clients;

    LoadClientsFromFile(CLIENTS_FILE_NAME, clients, "#//#");
    DetermineAccountFind(clients);
}

void ShowDeleteClientScreen()
{
    PrintScreenHeader("DELETE CLIENT");
    std::unordered_map<std::string, stClientData> clients;

    LoadClientsFromFile(CLIENTS_FILE_NAME , clients,  "#//#");
    DeleteClientByAccountNumber(clients, CLIENTS_FILE_NAME);
}

void ShowTransactionsMenu()
{
    cout << UI_LINE_BOUNDS << "\n";
    cout << right << setw(22) << "Transaction Menu" << "\n";
    cout << UI_LINE_BOUNDS << "\n";

    cout << right << setw(10) << "[1]" << " Deposit" << "\n";
    cout << right << setw(10) << "[2]" << " Withdraw" << "\n";
    cout << right << setw(10) << "[3]" << " Total Balances" << "\n";
    cout << right << setw(10) << "[4]" << " Return to Main Menu" << "\n";

    cout << UI_LINE_BOUNDS << "\n";
}

void DepositByAccNumber(unordered_map<string,stClientData> &clients, string fileName)
{
    string accountNumber = "";

    unordered_map<string, stClientData>::iterator clientIt;
    do
    {
        accountNumber = readAccountNumber();
        clientIt = FindClientByAccountNumber(accountNumber, clients);

        if (clientIt==clients.end())
            cout << "\nThe client with the account number[" << accountNumber << "] doesn't exist. Enter a different account number: ";

    } while (clientIt==clients.end());

    cout << "\nThe following are the client data: \n\n";
    PrintClientRecord(clientIt->second);

    int depositAmount = ReadDepositNumber();
    
    // A PIN-based verification would be better here. This'll be a place holder until I add PIN verification
    if (toupper(DetermineAgain("\nAre you sure you want to perform this transaction (Y/N)? ")) == 'Y')
    {
        clientIt->second.balanceUSD += depositAmount;

        cout << "\n\nDone! Your deposit of " << depositAmount << " has been added successfully. Your new balance is: "
            << clientIt->second.balanceUSD << " USD\n\n";
    }
    SaveToFile(fileName, clients);
}

void WithdrawByAccNumber(unordered_map<string, stClientData> &clients, string fileName)
{
    string accountNumber = "";
    // Could this logic be abstracted in a dedicated function because it exists in another function?

    unordered_map<string, stClientData>::iterator clientIt;
    do
    {
        accountNumber = readAccountNumber();
        clientIt = FindClientByAccountNumber(accountNumber, clients);

        if (clientIt == clients.end())
            cout << "\nThe client with the account number[" << accountNumber << "] doesn't exist. Enter a different account number: ";

    } while (clientIt == clients.end());

    cout << "\nThe following are the client data: \n\n";
    PrintClientRecord(clientIt->second);

    int withdrawAmount = ReadWithdrawNumber();

    // A PIN-based verification would be better here. This'll be a place holder until I add PIN verification
    VerifyBalanceForWithdraw(clientIt, withdrawAmount);
    SaveToFile(fileName, clients);
}

void DepositScreen(string fileName)
{
    PrintScreenHeader("DEPOSIT MONEY");
    unordered_map<string, stClientData> clients;

    LoadClientsFromFile(CLIENTS_FILE_NAME, clients, "#//#");
    DepositByAccNumber(clients, fileName);
}

void WithdrawScreen(string fileName)
{
    PrintScreenHeader("WITHDRAW MONEY");
    unordered_map<string, stClientData> clients;

    LoadClientsFromFile(CLIENTS_FILE_NAME, clients, "#//#");
    WithdrawByAccNumber(clients, fileName);
}

void PerformTransactionsMenuOption(const enTransactionsMenuChoice choice)
{
    switch (choice)
    {
    case enTransactionsMenuChoice::Deposit:
        ResetScreen();
        DepositScreen(CLIENTS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enTransactionsMenuChoice::Withdraw:
        ResetScreen();
        WithdrawScreen(CLIENTS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enTransactionsMenuChoice::TotalBalances:
        ResetScreen();
        ShowClientsBalances(CLIENTS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enTransactionsMenuChoice::MainMenu:
        break;
    }
}

void StartTransactionsMenu()
{
    enTransactionsMenuChoice RunningState = enTransactionsMenuChoice::MainMenu;
    do
    {
        ResetScreen();
        ShowTransactionsMenu();

        EvaluateMenuChoice(RunningState);

        if (RunningState != enTransactionsMenuChoice::MainMenu)
            PerformTransactionsMenuOption(RunningState);

    } while (RunningState != enTransactionsMenuChoice::MainMenu);

}

void PerformMainMenuOption(const enMenuChoice choice)
{
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
    case enMenuChoice::Transactions:
        ResetScreen();
        StartTransactionsMenu();
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
            PerformMainMenuOption(RunningState);

    } while (RunningState != enMenuChoice::Exit);
}

int main()
{
    StartBankSystem();
}
