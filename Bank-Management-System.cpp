#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

enum class enMenuChoice {ShowClients=1, AddClient=2, DeleteClient=3, UpdateClient=4, FindClient=5, Exit=6};
const string UI_LINE_BOUNDS(44, '=');

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
    long double balanceEGP = 0;
};

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

void ShowMainMenu()
{
    cout << UI_LINE_BOUNDS << "\n";
    cout << setw(22) << "MAIN MENU" << "\n";
    cout << UI_LINE_BOUNDS << "\n";

    cout << setw(10) << "[1]" << " Show Client List" << "\n";
    cout << setw(10) << "[2]" << " Add New Client" << "\n";
    cout << setw(10) << "[3]" << " Delete Client" << "\n";
    cout << setw(10) << "[4]" << " Update Client Info" << "\n";
    cout << setw(10) << "[5]" << " Find Client" << "\n";
    cout << setw(10) << "[6]" << " Exit" << "\n";

    cout << UI_LINE_BOUNDS << "\n";
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

vector<string> SplitString(string& S1, string delimiter = " ")
{
    vector<string> Words;
    short pos = 0;
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
        Words.push_back(sWord);
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
        client.balanceEGP = stoll(vClientData[4]);
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

void ShowClientList()
{

}

void RunRestOfMenuOptions(const enMenuChoice choice)
{
    vector<stClientData> clients = LoadFileContentsToVector("user_data.txt", "#//#");

    switch (choice)
    {
    case enMenuChoice::ShowClients:
        
        break;
    case enMenuChoice::AddClient:

        break;
    case enMenuChoice::DeleteClient:

        break;
    case enMenuChoice::UpdateClient:

        break;
    case enMenuChoice::FindClient:

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
        EvaluateMenuChoice(RunningState);

        if (RunningState != enMenuChoice::Exit)
            RunRestOfMenuOptions(RunningState);

        system("pause");

    } while (RunningState != enMenuChoice::Exit);
}

int main()
{

}
