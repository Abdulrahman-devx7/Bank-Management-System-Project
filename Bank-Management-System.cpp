#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <regex>
#include <cstdint>
#include <climits>
#include <limits>

using namespace std;
const string CLIENTS_FILE_NAME = "clients.txt";
const string USERS_FILE_NAME = "users.txt";
const string UI_LINE_BOUNDS(44, '=');
const int SCREEN_WIDTH = 125;

enum class enMenuChoice{
    ShowClients = 1,
    AddClient = 2,
    DeleteClient = 3,
    UpdateClient = 4,
    FindClient = 5,
    Transactions = 6,
    ManagerUsers = 7,
    Logout = 8
};

enum class enTransactionsMenuChoice {
    Deposit = 1,
    Withdraw = 2,
    TotalBalances = 3,
    MainMenu = 4
};

enum class enManageUsersMenuChoice {
    ListUsers = 1,
    AddUser = 2,
    DeleteUser = 3,
    UpdateUser = 4,
    FindUser = 5,
    MainMenu = 6
};

enum class enRunningState {
    LoginScreen,
    InsideBankSystem
};

enum class enUserPermissions : short {
    None = 0,  
    ListClients = 1,  
    AddClient = 2,  
    DeleteClient = 4,  
    UpdateClient = 8,  
    FindClient = 16, 
    Transactions = 32, 
    ManageUsers = 64, 
    All = -1  
};

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

struct stUserData 
{
    string user_name = "";
    string user_password = "";
    short permissions = 0;
};

struct stLoginCredentials
{
    string inputUsername;
    string inputPassword;
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

stClientData ConvertClientLineToRecord(string Line, string delimiter = "#//#")
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

        if (vClientData.size() >= 6)
            client.MarkForDelete = (vClientData[5] == "1");
    }

    return client;
}

stUserData ConvertUserLineToRecord(string line, string delimiter = "#//#")
{
    stUserData user;
    vector<string> userDataElements = SplitString(line, delimiter);

    if(userDataElements.size()>=3)
    {
        user.user_name = userDataElements[0];
        user.user_password = userDataElements[1];
        user.permissions = static_cast<short>(stoi(userDataElements[2]));
    }

    return user;
}
void ConvertToLowerCase(std::string &text)
{
    for (char& c : text) {
        c = tolower(static_cast<unsigned char>(c));
    }
}

void Trim(std::string& text, const string &toTrim = " \t\n\r")
{
    size_t start = text.find_first_not_of(toTrim);

    if (start == std::string::npos) {
        text.clear();
        return;
    }

    text.erase(0, start);

    size_t end = text.find_last_not_of(toTrim);

    text.erase(end + 1);
}

void LoadFromFile(string fileName, vector<stClientData>& clients, string delimiter = "#//#")
{
    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open())
    {
        string line = "";
        stClientData client;

        while (getline(file, line))
        {
            client = ConvertClientLineToRecord(line, delimiter);
            clients.push_back(client);
        }
        file.close();
    }
    else return;

}

void LoadFromFile(string fileName, unordered_map<string, stClientData>& clients, string delimiter = "#//#")
{
    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open())
    {
        string line = "";
        stClientData client;

        while (getline(file, line))
        {
            client = ConvertClientLineToRecord(line, delimiter);
            clients.insert({ client.accountNumber, client });
        }
        file.close();
    }
    else return;

}

void LoadFromFile(string fileName, unordered_map<string, stUserData>& clients, string delimiter = "#//#")
{
    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open())
    {
        string line = "";
        stUserData user;

        while (getline(file, line))
        {
            user = ConvertUserLineToRecord(line, delimiter);
            clients.insert({ user.user_name, user });
        }
        file.close();
    }
    else return;

}

void LoadFromFile(string fileName, vector<stUserData>& users, string delimiter = "#//#")
{
    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open())
    {
        string line = "";
        stUserData user;

        while (getline(file, line))
        {
            user = ConvertUserLineToRecord(line, delimiter);
            users.push_back(user);
        }
        file.close();
    }
    else return;
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

void ResetScreen()
{
    system("cls");
    system("color 0f");
}

//MORE MODULAR DESIGN FOR READING USERNAME & PASSWORD
string readAccountNumber()
{
    cout << "Please enter the account number: ";
    string userInput = "";
    cin >> userInput;

    return userInput;
}

string ReadUserName()
{
    string username = "";
    cout << "\nEnter the username: ";
    cin >> username;

    return username;
}

string ReadPassword()
{
    string password = "";
    cout << "\nEnter the password: ";
    cin >> password;

    return password;
}

stLoginCredentials ReadLoginCredentials()
{
    stLoginCredentials loginData;

    loginData.inputUsername = ReadUserName();
    ConvertToLowerCase(loginData.inputUsername);
    loginData.inputPassword = ReadPassword();

    return loginData;
}

// Instead of using a bool, why not introduce an Enum and a handful of checks inside this function for more state management over
// what has been attempted to take different actions depending on the state?
bool VerifyLogin(const stLoginCredentials &loginData, const unordered_map<string, stUserData> &users)
{
    if (users.find(loginData.inputUsername) == users.end())
        return false;
    else
    {
        auto fetchedPassword = users.find(loginData.inputUsername);
        return loginData.inputPassword == fetchedPassword->second.user_password;
    }
}

enUserPermissions GetRequiredPermissionForMenuChoice(const enMenuChoice menuChoice)
{

    switch (menuChoice)
    {
    case enMenuChoice::ShowClients:
        return enUserPermissions::ListClients;

    case enMenuChoice::AddClient:
        return enUserPermissions::AddClient;

    case enMenuChoice::DeleteClient:
        return enUserPermissions::DeleteClient;

    case enMenuChoice::UpdateClient:
        return enUserPermissions::UpdateClient;

    case enMenuChoice::FindClient:
        return enUserPermissions::FindClient;

    case enMenuChoice::Transactions:
        return enUserPermissions::Transactions;

    case enMenuChoice::ManagerUsers:
        return enUserPermissions::ManageUsers;

    case enMenuChoice::Logout:
        return enUserPermissions::None;

    default:
        return enUserPermissions::None;
    }
}

void DisplayAccessDenyMessage()
{
    cout << string(50, '-') << "\n";
    cout << setw(30) << right << "ACCESS DENIED\n";
    cout << setw(50) << right  << "You do NOT have a permission to use this utility\n";
    cout << setw(37) << right << " Please, contact your admin\n";
    cout << string(50, '-') << "\n";
}

void HandleUnauthorizedAccess()
{
    // A modular solution for everything
    // This can later scale up to do complex stuff to maybe the database, or lock an account 
    // after some actions
    ResetScreen();
    DisplayAccessDenyMessage();
    PromptUserToGetMenu();
}

bool VerifyPermission(const short userPermissions, enUserPermissions permissionToVerify)
{
    short permVal = static_cast<short>(permissionToVerify);
    return (userPermissions & permVal);
}

void NormalizeUsername(string& text)
{
    Trim(text);
    ConvertToLowerCase(text);
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

auto FindClientByAccountNumber(const string& userInputAccNumber, unordered_map<string, stClientData>& clients)
{
    auto clientIt = clients.find(userInputAccNumber);

    if (clientIt != clients.end() && clientIt->second.MarkForDelete)
        return clients.end();

    return clientIt;
}

void PrintInfoCard(const stClientData& data)
{
    //cout << "The following are the client's details\n";
    cout << UI_LINE_BOUNDS;

    cout << "\n" << left << setw(20) << "Account Number: " << data.accountNumber << "\n";
    cout << left << setw(20) << "PIN code: " << data.PIN_Number << "\n";
    cout << left << setw(20) << "Name: " << data.user_name << "\n";
    cout << left << setw(20) << "Phone Number: " << data.phoneNumber << "\n";
    cout << left << setw(20) << "Account Balance: " << data.balanceUSD << "\n";

    cout << UI_LINE_BOUNDS;
}

void PrintInfoCard(const stUserData& data)
{
    //cout << "The following are the user's details\n";
    cout << UI_LINE_BOUNDS;

    cout << "\n" << left << setw(20) << "User Name: " << data.user_name << "\n";
    cout << left << setw(20) << "Password: " << data.user_password << "\n";
    cout << left << setw(20) << "Permissions: " << data.permissions << "\n";

    cout << UI_LINE_BOUNDS;
}

void DetermineAccountFind(unordered_map<string, stClientData>& clients)
{
    string accountNumber = readAccountNumber();

    auto clientIt = FindClientByAccountNumber(accountNumber, clients);

    if (clientIt != clients.end())
    {
        cout << "\n";
        PrintInfoCard(clientIt->second);
    }
    else
        cout << "\nClient with the account number: " << accountNumber << " is not found!";
}

void MarkClientForDeleteByAccountNumber(unordered_map<string, stClientData>& clients, string accountNumber)
{
    clients[accountNumber].MarkForDelete = true;
}

bool CheckExistence(string accountNumber, unordered_map<string, stClientData>& clients)
{
    return clients.contains(accountNumber);
}

// Can we make this probably more usable or similar to DetermineAccountFind, which return a straight iterator to 
// user's location, which can be then stored in a variable to manipulate directly without needing to 
// search again to get a pointer
bool CheckExistence(string username, unordered_map<string, stUserData>& clients)
{
    return clients.contains(username);
}

void AssignPermissionsToUser(stUserData &userData)
{
    if (toupper(DetermineAgain("Do you want to give this user full access? y/n? ")) == 'Y')
    {
        userData.permissions = -1;
        return;
    }

    cout << "\n";
    cout << "Do you want to give access to :\n";
    cout << "\n";

    if (toupper(DetermineAgain("Do you want to give this user access to showing client list? y/n? ")) == 'Y')
        userData.permissions = userData.permissions | static_cast<short> (enUserPermissions::ListClients);

    cout << "\n";

    if(toupper(DetermineAgain("Do you want to give this user access to adding new clients? y/n? ")) == 'Y')
        userData.permissions = userData.permissions | static_cast<short> (enUserPermissions::AddClient);

    cout << "\n";

    if(toupper(DetermineAgain("Do you want to give this user access to deleting clients? y/n? ")) == 'Y')
        userData.permissions = userData.permissions | static_cast<short> (enUserPermissions::DeleteClient);

    cout << "\n";

    if(toupper(DetermineAgain("Do you want to give this user access to updating clients? y/n? ")) == 'Y')
        userData.permissions = userData.permissions | static_cast<short> (enUserPermissions::UpdateClient);

    cout << "\n";

    if(toupper(DetermineAgain("Do you want to give this user access to finding clients? y/n? ")) == 'Y')
        userData.permissions = userData.permissions | static_cast<short> (enUserPermissions::FindClient);

    cout << "\n";

    if(toupper(DetermineAgain("Do you want to give this user access to transactions? y/n? ")) == 'Y')
        userData.permissions = userData.permissions | static_cast<short> (enUserPermissions::Transactions);

    cout << "\n";

    if(toupper(DetermineAgain("Do you want to give this user access to managing users? y/n? ")) == 'Y')
        userData.permissions = userData.permissions | static_cast<short> (enUserPermissions::ManageUsers);

    cout << "\n";

}

void ReadClientDataUpdates(stClientData& data)
{
    if (toupper(DetermineAgain("\nUpdate PIN code? (Y/N)\n")) == 'Y')
    {
        cout << "Enter PIN code: ";
        getline(cin, data.PIN_Number);
    }

    if (toupper(DetermineAgain("\nUpdate name? (Y/N)\n")) == 'Y')
    {
        cout << "Enter name: ";
        getline(cin, data.user_name);
    }

    if (toupper(DetermineAgain("\nUpdate phone number? (Y/N)\n")) == 'Y')
    {
        cout << "Enter phone number: ";
        getline(cin, data.phoneNumber);
    }

    if (toupper(DetermineAgain("\nUpdate account balance? (Y/N)\n")) == 'Y')
    {
        cout << "Enter account balance: ";
        cin >> data.balanceUSD;
    }
}

void ReadUserUpdates(stUserData &user)
{
    if (toupper(DetermineAgain("\n\nDo you want to update the username (Y/N)?\n")) == 'Y')
        user.user_name = ReadUserName();

    if (toupper(DetermineAgain("\n\nDo you want to update the password (Y/N)?\n")) == 'Y')
        user.user_password = ReadPassword();

    AssignPermissionsToUser(user);
}

void readClientDataUpdates(stClientData& data)
{
    if (toupper(DetermineAgain("\nUpdate PIN code? (Y/N)\n")) == 'Y')
    {
        cout << "Enter PIN code: ";
        getline(cin, data.PIN_Number);
    }

    if (toupper(DetermineAgain("\nUpdate name? (Y/N)\n")) == 'Y')
    {
        cout << "Enter name: ";
        getline(cin, data.user_name);
    }

    if (toupper(DetermineAgain("\nUpdate phone number? (Y/N)\n")) == 'Y')
    {
        cout << "Enter phone number: ";
        getline(cin, data.phoneNumber);
    }

    if (toupper(DetermineAgain("\nUpdate account balance? (Y/N)\n")) == 'Y')
    {
        cout << "Enter account balance: ";
        cin >> data.balanceUSD;
    }
}


bool IsValidPIN(const string& PIN)
{
    return PIN.length() == 6 &&
        all_of(PIN.begin(), PIN.end(), ::isdigit);
}

bool isValidPhoneNumber(const string &phoneNumber)
{
    static const regex pattern("(010|011|012|015)[0-9]{8}");
    return regex_match(phoneNumber, pattern);
}

bool isValidAccountNumber(const string &accountNumber)
{
    static const regex pattern("[A-Z]{3}[0-9]{4}");
    return regex_match(accountNumber, pattern);
}

bool AreNamesOnlyLetters(const vector<string> &names)
{
    for (const string& name : names)
    {
        if (!all_of(name.begin(), name.end(), ::isalpha))
            return false;
    }
    return true;
}

void isValidFullName(stClientData& client)
{
    bool is4Names = true;
    bool isAllLetters = true;

    do
    {
        getline(cin, client.user_name);

        vector<string> Words = SplitString(client.user_name, " ");

        is4Names = (Words.size() == 4);
        isAllLetters = AreNamesOnlyLetters(Words);

        if (!is4Names || !isAllLetters)
            cout << "\nPlease, enter a valid full name form consisting of 4 names without any number or special characters!\n\n";

    } while (!isAllLetters || !is4Names);
}


//A lot of SRP and DRY needs to be discussed to remove redundancies here
void readClientData(stClientData& client, unordered_map<string, stClientData>& clients)
{
    cout << "Enter account number: ";

    bool isExistent = false;
    bool isAccNumberValid = true;
    do
    {
        getline(cin >> ws, client.accountNumber);
        isExistent = CheckExistence(client.accountNumber, clients);
        isAccNumberValid = isValidAccountNumber(client.accountNumber);

        if (isAccNumberValid)
        {
            if(isExistent)
                cout << "\nThe client with the account number[" << client.accountNumber << "] already exists, enter a different account number: ";
        }
        else 
            cout << "\nInput rejected! Please, enter an account number that follows the form: ABC1234 \n";

    } while (isExistent || !isAccNumberValid);

    cout << "\nEnter PIN code: ";
    bool isValid = true;
    do
    {
        getline(cin, client.PIN_Number);
        isValid = IsValidPIN(client.PIN_Number);

        if (!isValid)
            cout << "\nPlease enter a valid PIN (6 digits, numbers only)\n";
        
    } while (!isValid);

    cout << "\nEnter name: ";
    isValidFullName(client);

    cout << "\nEnter phone number: ";
    isValid = true;
    do
    {
        getline(cin, client.phoneNumber);
        isValid = isValidPhoneNumber(client.phoneNumber);

        if (!isValid)
            cout << "\nPlease enter a valid Egyptian phone number (11 digits, e.g. 01012345678)!\n";
    } while (!isValid);

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

vector<stClientData> GetVisibleClients(const vector<stClientData>& clients)
{
    vector<stClientData> visibleClients;
    for (const stClientData& client : clients)
    {
        if (!client.MarkForDelete)
            visibleClients.push_back(client);
    }
    return visibleClients;
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
    recordLine += to_string(data.balanceUSD) + delimiter;
    recordLine += (data.MarkForDelete ? "1" : "0");

    return recordLine;
}

string ConvertRecordToLine(const stUserData& userData, string delimiter = "#//#")
{
    string recordLine = "";

    recordLine += userData.user_name + delimiter;
    recordLine += userData.user_password + delimiter;
    recordLine += to_string(userData.permissions);

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

void AddNewClient(string fileName, unordered_map<string, stClientData>& clients)
{
    stClientData clientData;
    readClientData(clientData, clients);

    clients.insert({ clientData.accountNumber, clientData });
    AddDataLineToFile(fileName, ConvertRecordToLine(clientData));
}

// We should probably add a type of username validation that checks if the username doesn't contain special characters
// So only characters, numbers, hyphens, and underscores.
void readUserData(stUserData &userData, unordered_map<string, stUserData>& users)
{
    string inputUsername = "";

    bool isExistent = false;
    cout << "\nUsername: ";

    do
    {
        getline(cin >> ws, inputUsername);
        NormalizeUsername(inputUsername);

        if ((isExistent = CheckExistence(inputUsername, users)))
            cout << "\n The user with the username [" << inputUsername << "] already exists\n\n Enter a different username: ";

    } while (isExistent);
    userData.user_name = inputUsername;

    //Totally fragile for sure. It needs regex to make an obligatory standard form
    cout << "\nPassword: ";
    cin >> userData.user_password;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    AssignPermissionsToUser(userData);
}

void AddNewUser(string fileName, unordered_map<string, stUserData>& users)
{
    stUserData userData;
    readUserData(userData, users);

    users.insert({ userData.user_name, userData });
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
    cout << right << setw(10) << "[7]" << " Manage Users" << "\n";
    cout << right << setw(10) << "[8]" << " Logout" << "\n";

    cout << UI_LINE_BOUNDS << "\n";
}

void ShowManageUsersMenu()
{
    cout << UI_LINE_BOUNDS << "\n";
    cout << right << setw(30) << "Manage Users Menu" << "\n";
    cout << UI_LINE_BOUNDS << "\n";

    cout << right << setw(10) << "[1]" << " List Users" << "\n";
    cout << right << setw(10) << "[2]" << " Add User" << "\n";
    cout << right << setw(10) << "[3]" << " Delete User" << "\n";
    cout << right << setw(10) << "[4]" << " Update User" << "\n";
    cout << right << setw(10) << "[5]" << " Find User" << "\n";
    cout << right << setw(10) << "[6]" << " Return to Main Menu" << "\n";

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

void PrintFileInfoHeader(const vector<stUserData>& users)
{
    cout << right << setw(50) << "User list: (" << users.size() << ") user(s)" << "\n";
    cout << string(77, '-') << "\n";
    cout << left << "| " << setw(20) << " User Name " << "| ";
    cout << setw(30) << " Password " << "| ";
    cout << setw(20) << " Permissions " << "|" << "\n";
    cout << string(77, '-') << "\n";
}

void PrintClientBalancesHeader(const vector<stClientData>& clients)
{
    cout << right << setw(42) << "Balances List for (" << clients.size() << ") client(s)" << "\n";
    cout << string(SCREEN_WIDTH - 34, '-') << "\n";

    cout << left << "| " << setw(20) << " Account Number " << "| ";
    cout << setw(50) << " Client Name " << "|";
    cout << setw(14) << "Balance " << "|" << "\n";

    cout << string(SCREEN_WIDTH - 34, '-') << "\n";
}

//POTENTIAL DRY VIOLATION?
void EvaluateMenuChoice(enMenuChoice& menuChoice)
{
    stNumericInputData inputData;
    inputData.inputMessage = "Choose the operation you'd like to do (1-8)\n";
    inputData.from = 1;
    inputData.to = 8;
    inputData.validationErrorMessage = "\nPlease, enter a number in a valid range from the menu (1-8)!\n";

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

void EvaluateMenuChoice(enManageUsersMenuChoice& menuChoice)
{
    stNumericInputData inputData;
    inputData.inputMessage = "Choose the operation you'd like to do (1-6)\n";
    inputData.from = 1;
    inputData.to = 6;
    inputData.validationErrorMessage = "\nPlease, enter a number in a valid range from the menu (1-6)!\n";

    menuChoice = enManageUsersMenuChoice(ReadNumber(inputData));
}

void PrintIndividualTableInfo(const stClientData& client)
{
    cout << "| " << setw(20) << left << client.accountNumber << "| ";
    cout << setw(15) << left << client.PIN_Number << "| ";
    cout << setw(50) << left << client.user_name << "| ";
    cout << setw(15) << left << client.phoneNumber << "| ";
    cout << setw(14) << left << client.balanceUSD << "| " << "\n";
}

void PrintIndividualTableInfo(const stUserData& user)
{
    cout << "| " << setw(20) << left << user.user_name << "| ";
    cout << setw(30) << left << user.user_password << "| ";
    cout << setw(20) << left << user.permissions << "|" << "\n";
}

void PrintUserInfoInBalancesTable(const stClientData& client)
{
    cout << "| " << setw(20) << left << client.accountNumber << "| ";
    cout << setw(50) << left << client.user_name << "| ";
    cout << setw(14) << left << client.balanceUSD << "| " << "\n";
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

void SaveToFile(string fileName, unordered_map<string, stUserData>& users)
{
    fstream file;
    file.open(fileName, ios::out);

    if (file.is_open())
    {
        for (auto& [username, user] : users)
        {
            string line = ConvertRecordToLine(user);

            file << line << "\n";
        }
        file.close();
    }
}

void ShowClientListScreen(string fileName)
{
    vector<stClientData> clients;
    LoadFromFile(fileName, clients);

    vector<stClientData> visibleClients = GetVisibleClients(clients);

    PrintFileInfoHeader(visibleClients);

    if (visibleClients.size() == 0)
        cout << right << setw(62) << "NO CLIENTS ARE AVAILABLE IN THE SYSTEM!";

    for (const stClientData& client : visibleClients)
    {
        PrintIndividualTableInfo(client);
    }
    cout << string(SCREEN_WIDTH, '-') << "\n";
}

void ListUsersScreen(string fileName)
{
    vector<stUserData> users;
    LoadFromFile(fileName, users);

    PrintFileInfoHeader(users);

    if (users.size() == 0)
        cout << right << setw(50) << "NO USERS ARE AVAILABLE IN THE SYSTEM!";

    for (const stUserData& user : users)
        PrintIndividualTableInfo(user);

    cout << string(77, '-') << "\n";
}

void AddUserScreen(string fileName)
{
    PrintScreenHeader("ADD NEW USER");
    unordered_map<string, stUserData> users;
    LoadFromFile(fileName, users);

    do
    {
        AddNewUser(fileName, users);
        
        //You and I know that this is static and isn't linked into a succeed or fail :D I'll try to get into this if needed
        cout << "\nClient added successfully, ";

    } while (toupper(DetermineAgain("do you want to add more users (Y/N)? \n")=='Y'));
}

void DeleteUser(unordered_map<string, stUserData> &users, const string &fileName)
{
    stUserData userData;
    string username = ReadUserName();

    NormalizeUsername(username);

    if (CheckExistence(username, users))
    {
        userData = users.find(username)->second; 

        string foundUserName = users.find(username)->second.user_name;
        NormalizeUsername(foundUserName);

        if (foundUserName == "admin1")
        {
            cout << "\nYou cannot delete this user! \n\n";
            return;
        }
        else
        {
            PrintInfoCard(userData);
            if (toupper(DetermineAgain("\n\nAre you sure you want to delete this user (Y/N)?\n")) == 'Y')
            {
                users.erase(foundUserName);
                SaveToFile(fileName, users);
            }
        }
    }
    else
        cout << "The user with the username: [" << username << "\] is NOT found!\n";
}

void UpdateUser(unordered_map<string, stUserData>& users, const string& fileName)
{
    stUserData userData;
    string inputUsername = ReadUserName();

    NormalizeUsername(inputUsername);

    if (CheckExistence(inputUsername, users))
    {
        userData = users.find(inputUsername)->second;
        PrintInfoCard(userData);

        if (toupper(DetermineAgain("\n\nAre you sure you want to update this user (Y/N)?\n")) == 'Y')
        {
            ReadUserUpdates(userData);
            users.insert_or_assign(inputUsername, userData);
            SaveToFile(fileName, users);
        }
        else return;

    }
    else 
        cout << "The user with the username: [" << inputUsername << "\] is NOT found!\n";
}

void FindUser(unordered_map<string, stUserData>& users)
{
    string inputUsername = ReadUserName();

    if (CheckExistence(inputUsername, users))
        PrintInfoCard(users.find(inputUsername)->second);
    else
        cout << "The username: [" << inputUsername << "] has not been found!\n";
}

void DeleteUserScreen(string fileName)
{
    PrintScreenHeader("DELETE USER");
    unordered_map<string, stUserData> users;

    LoadFromFile(fileName, users);
    DeleteUser(users, fileName);
}

void UpdateUserScreen(string fileName)
{
    PrintScreenHeader("UPDATE USER");
    unordered_map<string, stUserData> users;

    LoadFromFile(fileName, users);
    UpdateUser(users, fileName);
}

void FindUserScreen(string fileName)
{
    PrintScreenHeader("FIND USER");
    unordered_map<string, stUserData> users;

    LoadFromFile(fileName, users);
    FindUser(users);
}

void ShowClientsBalances(string fileName)
{
    vector<stClientData> clients;
    LoadFromFile(fileName, clients);

    vector<stClientData> visibleClients = GetVisibleClients(clients);

    PrintClientBalancesHeader(visibleClients);

    if (visibleClients.size() == 0)
        cout << right << setw(62) << "NO CLIENTS ARE AVAILABLE IN THE SYSTEM!";

    for (const stClientData& client : visibleClients)
    {
        PrintUserInfoInBalancesTable(client);
    }
    cout << string(SCREEN_WIDTH - 34, '-') << "\n";
    cout << right << setw(42) << "Total Balances = " << AccumulateBalances(visibleClients) << " USD\n";
}

void VerifyBalanceForWithdraw(unordered_map<string, stClientData>::iterator& clientIt, int withdrawAmount)
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
    readClientDataUpdates(clientIt->second);
}

void UpdateClientByAccountNumber(unordered_map<string, stClientData>& clients, string fileName)
{
    stClientData Client;
    string accountNumber = readAccountNumber();

    auto clientIt = FindClientByAccountNumber(accountNumber, clients);

    if (clientIt != clients.end())
    {
        auto ClientIt = FindClientByAccountNumber(accountNumber, clients);
        Client = ClientIt->second;

        PrintInfoCard(Client);
        if (toupper(DetermineAgain("\n\nDo you want to update this client's data (Y/N)?\n")) == 'Y')
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

    if (FindClientByAccountNumber(accountNumber, clients) != clients.end())
    {
        auto ClientIt = FindClientByAccountNumber(accountNumber, clients);
        Client = ClientIt->second;

        PrintInfoCard(Client);
        if (toupper(DetermineAgain("\n\nAre you sure you want to delete this client (Y/N)?\n")) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(clients, accountNumber);
            SaveToFile(fileName, clients);

            cout << "\nAccount successfully deleted\n";
        }
    }
    else
        cout << "\nClient with account number (" << accountNumber << ") is not found!\n";
}

void LoginScreen(string fileName, enRunningState &runningState, stUserData &runningUser)
{
    unordered_map<string, stUserData> users;
    LoadFromFile(fileName, users, "#//#");

    bool isValidUsernameOrPass = true;
    stLoginCredentials loginDetails;

    do
    {
        ResetScreen();
        PrintScreenHeader("LOGIN SCREEN");

        if (!isValidUsernameOrPass)
        {
            cout << "Invalid Username or Password!\n";
            loginDetails = ReadLoginCredentials();
        }
        else
            loginDetails = ReadLoginCredentials();

    } while (!(isValidUsernameOrPass = VerifyLogin(loginDetails, users)));

    runningUser = users.find(loginDetails.inputUsername)->second;

    runningState = enRunningState::InsideBankSystem;
}

void AddClientsScreen(string fileName)
{
    PrintScreenHeader("ADD NEW CLIENT");

    unordered_map<string, stClientData> clients;
    LoadFromFile(fileName, clients);

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

    LoadFromFile(fileName, clients, "#//#");
    DeleteClientByAccountNumber(clients, fileName);
}

void UpdateClientScreen(string fileName)
{
    PrintScreenHeader("UPDATE CLIENT INFO");
    unordered_map<string, stClientData> clients;

    LoadFromFile(fileName, clients, "#//#");
    UpdateClientByAccountNumber(clients, fileName);
}

void FindClientScreen(string fileName)
{
    PrintScreenHeader("FIND CLIENT");
    unordered_map<string, stClientData> clients;

    LoadFromFile(fileName, clients, "#//#");
    DetermineAccountFind(clients);
}

void ShowDeleteClientScreen(string fileName)
{
    PrintScreenHeader("DELETE CLIENT");
    std::unordered_map<std::string, stClientData> clients;

    LoadFromFile(fileName, clients, "#//#");
    DeleteClientByAccountNumber(clients, fileName);
}

void ShowTransactionsMenu()
{
    cout << UI_LINE_BOUNDS << "\n";
    cout << right << setw(30) << "Transaction Menu" << "\n";
    cout << UI_LINE_BOUNDS << "\n";

    cout << right << setw(10) << "[1]" << " Deposit" << "\n";
    cout << right << setw(10) << "[2]" << " Withdraw" << "\n";
    cout << right << setw(10) << "[3]" << " Total Balances" << "\n";
    cout << right << setw(10) << "[4]" << " Return to Main Menu" << "\n";

    cout << UI_LINE_BOUNDS << "\n";
}

void DepositByAccNumber(unordered_map<string, stClientData>& clients, string fileName)
{
    string accountNumber = "";

    unordered_map<string, stClientData>::iterator clientIt;
    do
    {
        accountNumber = readAccountNumber();
        clientIt = FindClientByAccountNumber(accountNumber, clients);

        if (clientIt == clients.end())
            cout << "\nThe client with the account number[" << accountNumber << "] doesn't exist. Enter a different account number: ";

    } while (clientIt == clients.end());

    cout << "\nThe following are the client data: \n\n";
    PrintInfoCard(clientIt->second);

    int depositAmount = ReadDepositNumber();

    // A PIN-based verification would be better here. This'll be a place holder until I add PIN verification
    VerifyDeposit(clientIt, depositAmount);
    SaveToFile(fileName, clients);
}

void WithdrawByAccNumber(unordered_map<string, stClientData>& clients, string fileName)
{
    string accountNumber = "";

    unordered_map<string, stClientData>::iterator clientIt;
    do
    {
        accountNumber = readAccountNumber();
        clientIt = FindClientByAccountNumber(accountNumber, clients);

        if (clientIt == clients.end())
            cout << "\nThe client with the account number[" << accountNumber << "] doesn't exist. Enter a different account number: ";

    } while (clientIt == clients.end());

    cout << "\nThe following are the client data: \n\n";
    PrintInfoCard(clientIt->second);

    int withdrawAmount = ReadWithdrawNumber();

    VerifyBalanceForWithdraw(clientIt, withdrawAmount);
    SaveToFile(fileName, clients);
}

void DepositScreen(string fileName)
{
    PrintScreenHeader("DEPOSIT MONEY");
    unordered_map<string, stClientData> clients;

    LoadFromFile(fileName, clients, "#//#");
    DepositByAccNumber(clients, fileName);
}

void WithdrawScreen(string fileName)
{
    PrintScreenHeader("WITHDRAW MONEY");
    unordered_map<string, stClientData> clients;

    LoadFromFile(fileName, clients, "#//#");
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

void PerformManageUsersMenuOption(const enManageUsersMenuChoice choice)
{
    switch (choice)
    {
    case enManageUsersMenuChoice::ListUsers:
        ResetScreen();
        ListUsersScreen(USERS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enManageUsersMenuChoice::AddUser:
        ResetScreen();
        AddUserScreen(USERS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enManageUsersMenuChoice::DeleteUser:
        ResetScreen();
        DeleteUserScreen(USERS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enManageUsersMenuChoice::UpdateUser:
        ResetScreen();
        UpdateUserScreen(USERS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enManageUsersMenuChoice::FindUser:
        ResetScreen();
        FindUserScreen(USERS_FILE_NAME);
        PromptUserToGetMenu();
        break;

    case enManageUsersMenuChoice::MainMenu:
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

void StartManageUsers()
{
    enManageUsersMenuChoice RunningState = enManageUsersMenuChoice::MainMenu;
    do
    {
        ResetScreen();
        ShowManageUsersMenu();

        EvaluateMenuChoice(RunningState);

        if (RunningState != enManageUsersMenuChoice::MainMenu)
            PerformManageUsersMenuOption(RunningState);

    } while (RunningState != enManageUsersMenuChoice::MainMenu);

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
        ShowDeleteClientScreen(CLIENTS_FILE_NAME);
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

    case enMenuChoice::ManagerUsers:
        ResetScreen();
        StartManageUsers();
        PromptUserToGetMenu();
        break;

    case enMenuChoice::Logout:
        break;
    }
}

void RunBankServices(enRunningState &state, const stUserData &runningUser)
{
    enMenuChoice runningUtility = enMenuChoice::Logout;

    do
    {
        ResetScreen();
        ShowMainMenu();

        EvaluateMenuChoice(runningUtility);

        if (runningUtility != enMenuChoice::Logout)
        {
            enUserPermissions requiredPermission = GetRequiredPermissionForMenuChoice(runningUtility);
            if (VerifyPermission(runningUser.permissions, requiredPermission))
                PerformMainMenuOption(runningUtility);
            else
                HandleUnauthorizedAccess();
        }

    } while (runningUtility != enMenuChoice::Logout);

    state = enRunningState::LoginScreen;
}

void StartBankSystem()
{
    //Default Start
    enRunningState RunningState = enRunningState::LoginScreen;
    stUserData RunningUser;
    do
    {
        if (RunningState == enRunningState::LoginScreen)
            LoginScreen(USERS_FILE_NAME, RunningState, RunningUser);

        if(RunningState== enRunningState::InsideBankSystem)
            RunBankServices(RunningState, RunningUser);

    } while (true);
}

int main()
{
    StartBankSystem();
}
