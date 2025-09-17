#include <iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<fstream>
#include<cctype>
using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";
const string Seperator = "###";
struct stClient_Data {
    string Account_Number;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};
struct stUser_Data {
    string Name;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};
stUser_Data CurrentUser;
enum EnMainMenueOptions {
    eShowClientList = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6,
    eManageUsers = 7, eLogout = 8
};
enum EnUserMenueOptions {
    eShowUsersList = 1, eAddNewUser = 2,
    eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMainMenue = 6
};
enum UserPermissions {
    eAll = -1, EShowClientsList = 1, EAddNewClient = 2,
    EDeleteClient = 4, EUpdateClient = 8,
    EFindClient = 16, ETransactions = 32,
    EManageUsers = 64
};
enum EnTransaction {
    Deposite = 1,Withdraw = 2,
    Total_Balance = 3, MainMenue = 4
};

//Functions Declaration
void ShowMainMenueScreen();
void ShowTransactionMenueOptions();
void ShowManageUsersMenueOptions();
void Login();
void  GoBackToMainMenue();
void GoBackToUserMainMenue();
short ReadMainMenueOptions()
{
    short YourChoice;
    do
    {
        cout << "Choose What Do You Want To Do? [1 to 7]? "; cin >> YourChoice;
    } while (YourChoice < 1 || YourChoice>8);
    return YourChoice;
}
short ReadUserMenueOptions()
{
    short YourChoice;
    do
    {
        cout << "Choose What Do You Want To Do? [1 to 6]? "; cin >> YourChoice;
    } while (YourChoice < 1 || YourChoice>6);
    return YourChoice;
}
vector<string>SplitWords(string s)
{
    vector<string>words;
    short pos;
    string word;
    while ((pos = s.find(Seperator)) != s.npos)
    {
        word = s.substr(0, pos);
        if (word != "")
        {
            words.push_back(word);
        }
        s.erase(0, pos + Seperator.length());
    }
    if (s != "")
    {
        words.push_back(s);
    }
    return words;
}
stClient_Data ConvertClientLineToRecord(string s)
{
    vector<string>WordsOfLine = SplitWords(s);
    stClient_Data Client;
    Client.Account_Number = WordsOfLine[0];
    Client.PinCode = WordsOfLine[1];
    Client.Name = WordsOfLine[2];
    Client.Phone = WordsOfLine[3];
    Client.AccountBalance = stod(WordsOfLine[4]);
    return Client;
}
stUser_Data ConvertUserLineToRecord(string s)
{
    vector<string>WordsOfLine = SplitWords(s);
    stUser_Data User;
    User.Name = WordsOfLine[0];
    User.Password = WordsOfLine[1];
    User.Permissions = stoi(WordsOfLine[2]);
    return User;
}
string ConvertClientRecordToLine(stClient_Data Client)
{
    string s = "";
    s += Client.Account_Number + Seperator;
    s += Client.PinCode + Seperator;
    s += Client.Name + Seperator;
    s += Client.Phone + Seperator;
    s += to_string(Client.AccountBalance);
    return s;
}
string ConvertUserRecordToLine(stUser_Data User)
{
    string s = "";
    s += User.Name + Seperator;
    s += User.Password + Seperator;
    s += to_string(User.Permissions)+ Seperator;
    return s;
}
vector <stClient_Data> LoadClientsFromFile()
{
    vector<stClient_Data>AllClients;
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        stClient_Data Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertClientLineToRecord(Line);
            AllClients.push_back(Client);
        }
        MyFile.close();
    }
    return AllClients;
}
vector <stUser_Data> LoadUsersFromFile()
{
    vector<stUser_Data>AllUsers;
    fstream MyFile;
    MyFile.open(UsersFileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        stUser_Data User;
        while (getline(MyFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            AllUsers.push_back(User);
        }
        MyFile.close();
    }
    return AllUsers;
}
void SaveClientDataToFile(vector<stClient_Data>AllClients)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClient_Data C : AllClients)
        {
            if (C.MarkForDelete == false)
            {
                MyFile << ConvertClientRecordToLine(C) << endl;
            }
        }
        MyFile.close();
    }
}
void SaveUserDataToFile(vector<stUser_Data>AllUsers)
{
    fstream MyFile;
    MyFile.open(UsersFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stUser_Data U : AllUsers)
        {
            if (U.MarkForDelete == false)
            {
                MyFile << ConvertUserRecordToLine(U) << endl;
            }
        }
        MyFile.close();
    }
}
void AddClientToFile(string s)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << s << endl;
    }
    MyFile.close();
}
void AddUserToFile(string s)
{
    fstream MyFile;
    MyFile.open(UsersFileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << s << endl;
    }
    MyFile.close();
}
void  PrintClientsInScreen(stClient_Data Client)
{
    cout << "| " << setw(17) << left << Client.Account_Number;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(30) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(10) << left << Client.AccountBalance;
}
void  PrintUsersInScreen(stUser_Data User)
{
    cout << "| " << setw(17) << left << User.Name;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(30) << left << User.Permissions;
}
bool CheckUserPermissions(UserPermissions Permission)
{
    if (CurrentUser.Permissions == UserPermissions::eAll)
    {
        return true;
    }
    if ((Permission & CurrentUser.Permissions) == Permission)
    {
        return true;
    }

    return false;
}
void ShowDeniedScreen()
{
    cout << "========================================================\n";
    cout << "\nAccess to this screen is denied,if you a permission to this\nscreen contact your admin please \n";
    cout << "\n========================================================\n";
    GoBackToMainMenue();
}
void  ShowClientList()
{
    if (!CheckUserPermissions(UserPermissions::EShowClientsList))
    {
        ShowDeniedScreen();
        
        return;
    }
    vector<stClient_Data> AllClients = LoadClientsFromFile();
    cout << "\n\t\t\t\tClient List (" << AllClients.size() << ") Client(s).\n";
    cout << "_______________________________________________";
    cout << "_____________________________________________\n\n";
    cout << "| " << setw(17) << left << " Account Number";
    cout << "| " << setw(10) << left << " Pin Code";
    cout << "| " << setw(30) << left << " Client Name";
    cout << "| " << setw(12) << left << " Phone";
    cout << "| " << setw(10) << left << " Balance";
    cout << "\n_______________________________________________";
    cout << "_____________________________________________\n\n";
    if (AllClients.size() == 0)
    {
        cout << "\n\nThere Is No Available Client In The System...\n";
    }
    else
    {
        for (stClient_Data& Client : AllClients)
        {
            PrintClientsInScreen(Client);
            cout << endl;
        }
    }
    cout << "_______________________________________________";
    cout << "_____________________________________________\n\n";
}
void  ShowUserList()
{
   
    vector<stUser_Data> AllUsers = LoadUsersFromFile();
    cout << "\n\t\t\t\tUser List (" << AllUsers.size() << ") User(s).\n";
    cout << "_______________________________________________";
    cout << "_____________________________________________\n\n";
    cout << "| " << setw(17) << left << " Name";
    cout << "| " << setw(10) << left << " Password";
    cout << "| " << setw(30) << left << " Permissions";
    cout << "\n_______________________________________________";
    cout << "_____________________________________________\n\n";
    if (AllUsers.size() == 0)
    {
        cout << "\n\nThere Is No Available User In The System...\n";
    }
    else
    {
        for (stUser_Data& User : AllUsers)
        {
            PrintUsersInScreen(User);
            cout << endl;
        }
    }
    cout << "_______________________________________________";
    cout << "_____________________________________________\n\n";
}
bool IsClientAlreadyExist(string Account_Number)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::in);
    if (MyFile.is_open())
    {
        stClient_Data Client;
        string Line;
        while (getline(MyFile, Line))
        {
            Client = ConvertClientLineToRecord(Line);
            if (Client.Account_Number == Account_Number)
            {
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}
bool IsUserAlreadyExist(string UserName)
{
    fstream MyFile;
    MyFile.open(UsersFileName, ios::in);
    if (MyFile.is_open())
    {
        stUser_Data User;
        string Line;
        while (getline(MyFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            if (User.Name==UserName)
            {
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}
short ReadUserPermissions()
{
    short Permissions=0;
    char AdminChoice;
    cout << "Do you want to give this user all permissions : (Y or N ) : ";
    cin >> AdminChoice;
    if (AdminChoice == 'y' || AdminChoice == 'Y')
    {
        Permissions = -1;
        return Permissions;
    }
    cout << "Do you want to give this user the permission to Show client List : (Y or N ) : ";
    cin >> AdminChoice;
    if (AdminChoice == 'y' || AdminChoice == 'Y')
    {
        Permissions += 1;
    }
    cout << "Do you want to give this user the permission to add a new client : (Y or N ) : ";
    cin >> AdminChoice;
    if (AdminChoice == 'y' || AdminChoice == 'Y')
    {
        Permissions +=2 ;
    }
    cout << "Do you want to give this user the permission to delete a client : (Y or N ) : ";
    cin >> AdminChoice;
    if (AdminChoice == 'y' || AdminChoice == 'Y')
    {
        Permissions += 4;
    }
    cout << "Do you want to give this user the permission to update a client : (Y or N ) : ";
    cin >> AdminChoice;
    if (AdminChoice == 'y' || AdminChoice == 'Y')
    {
        Permissions += 8;
    }cout << "Do you want to give this user the permission to find a client : (Y or N ) : ";
    cin >> AdminChoice;
    if (AdminChoice == 'y' || AdminChoice == 'Y')
    {
        Permissions += 16;
    }cout << "Do you want to give this user the permission to do a transaction : (Y or N ) : ";
    cin >> AdminChoice;
    if (AdminChoice == 'y' || AdminChoice == 'Y')
    {
        Permissions += 32;
    }cout << "Do you want to give this user the permission to manage users : (Y or N ) : ";
    cin >> AdminChoice;
    if (AdminChoice == 'y' || AdminChoice == 'Y')
    {
        Permissions += 64;
    }

    return Permissions;

}
stClient_Data ReadClientInfo()
{
    stClient_Data Client;
    cout << "Enter Account Number ? "; getline(cin >> ws, Client.Account_Number);
    while (IsClientAlreadyExist(Client.Account_Number))
    {
        cout << "Client With [" << Client.Account_Number << "] Already Exists, Enter Another Account Number ? ";
        getline(cin >> ws, Client.Account_Number);
    }
    cout << "Enter PinCode : "; getline(cin >> ws, Client.PinCode);
    cout << "Enter Name : "; getline(cin, Client.Name);
    cout << "Enter Phone : "; getline(cin, Client.Phone);
    cout << "Enter AccountBalance : "; cin >> Client.AccountBalance;
    return Client;
}
stUser_Data ReadUserInfo()
{
    stUser_Data User;
    cout << "Enter User Name : "; getline(cin >> ws, User.Name);
    while (IsUserAlreadyExist(User.Name))
    {
        cout << "User With [" << User.Name << "] Already Exists, Enter Another Name ? ";
        getline(cin >> ws, User.Name);
    }
    cout << "Enter Password : "; getline(cin,User.Password);
    User.Permissions = ReadUserPermissions();
    return User;
}
void  AddNewClients()
{
    char AdditionalChoic;
    stClient_Data Client;
    Client = ReadClientInfo();
    AddClientToFile(ConvertClientRecordToLine(Client));
}
void  AddNewUsers()
{
    char AdditionalChoic;
    stUser_Data User;
    User = ReadUserInfo();
    AddUserToFile(ConvertUserRecordToLine(User));
}
void ShowAddNewClientScreen()
{
    if (!CheckUserPermissions(UserPermissions::EAddNewClient))
    {
        ShowDeniedScreen();

        return;
    }
    char AdditionalChoic = 'n';
    do
    {
        system("cls");
        cout << "----------------------------------------\n";
        cout << "\tAdd New Client Screen\n";
        cout << "----------------------------------------\n\n";
        cout << "Add New Client :\n\n";
        AddNewClients();
        cout << "Client Added Successfully, Do You Want To Add More Clients ? (Y/N) ? ";
        cin >> AdditionalChoic;
    } while (toupper(AdditionalChoic) == 'Y');
}
void ShowAddNewUserScreen()
{
    char AdditionalChoic = 'n';
    do
    {
        system("cls");
        cout << "----------------------------------------\n";
        cout << "\tAdd New User Screen\n";
        cout << "----------------------------------------\n\n";
        cout << "Add New User :\n\n";
        AddNewUsers();
        cout << "User Added Successfully, Do You Want To Add More Users ? (Y/N) ? ";
        cin >> AdditionalChoic;
    } while (toupper(AdditionalChoic) == 'Y');
}
void  PrintClientCard(stClient_Data Client)
{
    cout << "\nThe Following Are The Client Details : \n";
    cout << "-------------------------------------------\n";
    cout << "Account Number  : " << Client.Account_Number << endl;
    cout << "Pin Code        : " << Client.PinCode << endl;
    cout << "Name            : " << Client.Name << endl;
    cout << "Phone           : " << Client.Phone << endl;
    cout << "Account Balance : " << Client.AccountBalance << endl;
    cout << "-------------------------------------------\n";
}
void  PrintUserCard(stUser_Data User)
{
    cout << "\nThe Following Are The User Details : \n";
    cout << "-------------------------------------------\n";
    cout << "User Name       : " << User.Name << endl;
    cout << "Password        : " << User.Password<< endl;
    cout << "Permissions     : " << User.Permissions << endl;
    cout << "-------------------------------------------\n";
}
void MarkForDeleteOrUpdateClientRecord(vector<stClient_Data>& AllClients, string Account_Number)
{

    for (stClient_Data& Client : AllClients)
    {
        if (Client.Account_Number == Account_Number)
        {
            Client.MarkForDelete = true;
            break;
        }
    }
}
void MarkForDeleteOrUpdateUserRecord(vector<stUser_Data>& AllUsers, string UserName)
{

    for (stUser_Data& User : AllUsers)
    {
        if (User.Name == UserName)
        {
            User.MarkForDelete = true;
            break;
        }
    }
}
bool FindClientByAccountNumber(vector<stClient_Data>AllClients, stClient_Data& Client, string Account_Number)
{
    for (stClient_Data C : AllClients)
    {
        if (C.Account_Number == Account_Number)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
bool FindUserByUserName(vector<stUser_Data>AllUsers, stUser_Data& User, string UserName)
{
    for (stUser_Data U : AllUsers)
    {
        if (U.Name == UserName)
        {
            User = U;
            return true;
        }
    }
    return false;
}
bool FindUserByUserNameAndPassword(vector<stUser_Data>AllUsers, stUser_Data& User, string UserName,string Password)
{
    for (stUser_Data U : AllUsers)
    {
        if (U.Name == UserName&&U.Password==Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}
string ReadClientAccountNumber()
{
    string Account_Number;
    cout << "Enter Account Number ? "; cin >> Account_Number;
    return Account_Number;
}
string ReadUserName()
{
    string UserName;
    cout << "Enter User Name please : ? "; cin >> UserName;
    return UserName;
}
void DeleteClientByAccountNumber(vector<stClient_Data> AllClients, string Account_Number)
{
    stClient_Data Client;
    char SureChoice = 'n';
    if (FindClientByAccountNumber(AllClients, Client, Account_Number))
    {
        PrintClientCard(Client);
        cout << "Are You Sure You Want To Delete This Client ? (Y/N) ? "; cin >> SureChoice;
        if (toupper(SureChoice) == 'Y')
        {
            MarkForDeleteOrUpdateClientRecord(AllClients, Account_Number);
            SaveClientDataToFile(AllClients);
            cout << "\nClient Deleted Succefully.\n";
        }
    }
    else
    {
        cout << "Client With Account Number (" << Account_Number << ") Is Not Found.\n";
    }

}
void DeleteUserByUserName(vector<stUser_Data> AllUsers, string UserName)
{
    stUser_Data User;
    char SureChoice = 'n';
    if (FindUserByUserName(AllUsers, User, UserName))
    {
        PrintUserCard(User);
        cout << "Are You Sure You Want To Delete This User ? (Y/N) ? "; cin >> SureChoice;
        if (toupper(SureChoice) == 'Y')
        {
            MarkForDeleteOrUpdateUserRecord(AllUsers, UserName);
            SaveUserDataToFile(AllUsers);
            cout << "\nUser Deleted Succefully.\n";
        }
    }
    else
    {
        cout << "User with User Name (" << UserName << ") Is Not Found.\n";
    }

}
void ShowDeleteClientScreen()
{
    if (!CheckUserPermissions(UserPermissions::EDeleteClient))
    {
        ShowDeniedScreen();

        return;
    }
    string Account_Number;
    cout << "----------------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "----------------------------------------\n\n";
    cout << "Delete Client :\n\n";
    vector<stClient_Data> AllClients = LoadClientsFromFile();
    Account_Number = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AllClients, Account_Number);
}
void ShowDeleteUserScreen()
{
    string UserName;
    cout << "----------------------------------------\n";
    cout << "\tDelete User Screen\n";
    cout << "----------------------------------------\n\n";
    cout << "Delete User :\n\n";
    vector<stUser_Data> AllUsers = LoadUsersFromFile();
    UserName = ReadUserName();
    DeleteUserByUserName(AllUsers, UserName);
}
stClient_Data ReadUpdateInfoForClient(string AccountNumber)
{
    stClient_Data Client_Data;
    Client_Data.Account_Number = AccountNumber;
    cout << "Enter PinCode : "; getline(cin >> ws, Client_Data.PinCode);
    cout << "Enter Name : "; getline(cin, Client_Data.Name);
    cout << "Enter Phone : "; getline(cin, Client_Data.Phone);
    cout << "Enter AccountBalance : "; cin >> Client_Data.AccountBalance;
    return Client_Data;
}
stUser_Data ReadUpdateInfoForUser(string UserName,short Permissions)
{
    stUser_Data User_Data;
    User_Data.Name = UserName;
    cout << "Enter Password : "; getline(cin>>ws, User_Data.Password);
    User_Data.Permissions = Permissions;
    return User_Data;
}
void UpdateClientInfoByAccountNumber(vector<stClient_Data>& AllClients, stClient_Data Client)
{
    for (stClient_Data& C : AllClients)
    {
        if (C.MarkForDelete == true)
        {
            C = Client;
            break;
        }
    }
}
void UpdateUserInfoByAccountNumber(vector<stUser_Data>& AllUsers, stUser_Data User)
{
    for (stUser_Data& U : AllUsers)
    {
        if (U.MarkForDelete == true)
        {
            U = User;
            break;
        }
    }
}
void  UpdateClientByAccountNumber(vector<stClient_Data> AllClients, string Account_Number)
{
    stClient_Data Client;
    char SureChoice = 'n';
    if (FindClientByAccountNumber(AllClients, Client, Account_Number))
    {
        PrintClientCard(Client);
        cout << "Are You Sure You Want To Update This Client ? (Y/N) ? "; cin >> SureChoice;
        if (toupper(SureChoice) == 'Y')
        {
            MarkForDeleteOrUpdateClientRecord(AllClients, Account_Number);
            UpdateClientInfoByAccountNumber(AllClients, ReadUpdateInfoForClient(Account_Number));
            SaveClientDataToFile(AllClients);
            cout << "\nClient Updateded Succefully.\n";
        }
    }
    {
        cout << "\nClient With Account_Number [" << Account_Number << "] not exist !\n";
        GoBackToMainMenue();
    }
}
void  UpdateUserByUserName(vector<stUser_Data> AllUsers, string UserName)
{
    stUser_Data User;
    char SureChoice = 'n';
    if (FindUserByUserName(AllUsers, User, UserName))
    {
        PrintUserCard(User);
        cout << "Are You Sure You Want To Update This User ? (Y/N) ? "; cin >> SureChoice;
        if (toupper(SureChoice) == 'Y')
        {
            MarkForDeleteOrUpdateUserRecord(AllUsers, UserName);
            UpdateUserInfoByAccountNumber(AllUsers, ReadUpdateInfoForUser(User.Name,User.Permissions));
            SaveUserDataToFile(AllUsers);
            cout << "\nUser Updateded Succefully.\n";
        }
    }
    else
    {
        cout << "\nUser With User Name [" << UserName << "] not exist !\n";
        GoBackToUserMainMenue();
    }
}
void ShowUpdateClientScreen()
{
    if (!CheckUserPermissions(UserPermissions::EUpdateClient))
    {
        ShowDeniedScreen();

        return;
    }
    string Account_Number;
    cout << "----------------------------------------\n";
    cout << "\tUpdate Client Screen\n";
    cout << "----------------------------------------\n\n";
    vector<stClient_Data> AllClients = LoadClientsFromFile();
    Account_Number = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AllClients, Account_Number);
}
void ShowUpdateUserScreen()
{
    string UserName;
    cout << "----------------------------------------\n";
    cout << "\tUpdate User Screen\n";
    cout << "----------------------------------------\n\n";
    vector<stUser_Data> AllUsers = LoadUsersFromFile();
    UserName = ReadUserName();
    UpdateUserByUserName(AllUsers, UserName);
}
void GoBackToMainMenue()
{
    cout << "\nPress Any Key To Go Back To Main Menue...\n";
    system("pause>0");
    ShowMainMenueScreen();
}
void GoBackToUserMainMenue()
{
    cout << "\nPress Any Key To Go Back To Main Menue...\n";
    system("pause>0");
    ShowManageUsersMenueOptions();
}
void  FindClient()
{
    if (!CheckUserPermissions(UserPermissions::EFindClient))
    {
        ShowDeniedScreen();

        return;
    }
    string Account_Number;
    vector<stClient_Data> AllClients = LoadClientsFromFile();
    stClient_Data Client;
    cout << "----------------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "----------------------------------------\n\n";
    Account_Number = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AllClients, Client, Account_Number))
    {
        PrintClientCard(Client);
    }
    else
    {
        cout << "Client With Account Number [" << Account_Number << "] Is Not Found !\n";
    }
}
void  FindUser()
{
    string UserName;
    vector<stUser_Data> AllUsers = LoadUsersFromFile();
    stUser_Data User;
    cout << "----------------------------------------\n";
    cout << "\tFind User Screen\n";
    cout << "----------------------------------------\n\n";
    UserName = ReadUserName();
    if (FindUserByUserName(AllUsers, User, UserName))
    {
        PrintUserCard(User);
    }
    else
    {
        cout << "User With User Name [" << UserName << "] Is Not Found !\n";
    }
}
short ReadTransactionMenueOptions()
{
    short YourTransactionChoice = 0;
    do
    {
        cout << "Choose What Do You Want To Do [1 To 4] ? "; cin >> YourTransactionChoice;
    } while (YourTransactionChoice < 1 || YourTransactionChoice>4);
    return YourTransactionChoice;
}
void GoBackToTransactionMenue()
{
    cout << "Press Any Key To Go Back To Transaction Menue...\n";
    system("pause>0");
    ShowTransactionMenueOptions();
}
bool DepositeFunction(vector<stClient_Data>& AllClients, double DepositeAmount, string Account_Number)
{
    char SureChoice = 'n';
    cout << "Are You Sure You Want To Perform This Transaction ? (Y/N) ?"; cin >> SureChoice;
    if (toupper(SureChoice) == 'Y')
    {
        for (stClient_Data& C : AllClients)
        {
            if (C.Account_Number == Account_Number)
            {
                C.AccountBalance += DepositeAmount;
                SaveClientDataToFile(AllClients);
                cout << "Done Succefully-):\n";
                cout << "New Balance Is : " << C.AccountBalance << endl;
                return true;
            }
        }
    }
    return false;
}
void ShowDepositeScreen()
{
    cout << "----------------------------------------\n";
    cout << "\tDeposite Screen\n";
    cout << "----------------------------------------\n\n";
    vector<stClient_Data> AllClients = LoadClientsFromFile();
    string Account_Number = ReadClientAccountNumber();
    stClient_Data Client;
    while (!FindClientByAccountNumber(AllClients, Client, Account_Number))
    {
        cout << "Client With Account Number [" << Account_Number << "] Is Not Exiet, ";
        Account_Number = ReadClientAccountNumber();
    }
    PrintClientCard(Client);
    double DepositeAmount;
    cout << "\nPlease Enter Deposite Amount ? "; cin >> DepositeAmount;
    DepositeFunction(AllClients, DepositeAmount, Account_Number);
}
bool CheckBalance(vector<stClient_Data> AllClients, string Account_Number, int WithdrawAmount)
{
    for (stClient_Data& C : AllClients)
    {
        if (C.Account_Number == Account_Number)
        {
            if (WithdrawAmount > C.AccountBalance)
            {
                return false;
            }
        }
    }
    return true;
}
void ShowWithDrawScreen()
{
    cout << "----------------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "----------------------------------------\n\n";
    vector<stClient_Data> AllClients = LoadClientsFromFile();
    string Account_Number = ReadClientAccountNumber();
    stClient_Data Client;
    while (!FindClientByAccountNumber(AllClients, Client, Account_Number))
    {
        cout << "Client With Account Number [" << Account_Number << "] Is Not Exiet, ";
        Account_Number = ReadClientAccountNumber();
    }
    PrintClientCard(Client);
    double WithdrawAmount;
    cout << "Enter Withdraw Amount : ";
    cin >> WithdrawAmount;
    if (WithdrawAmount > Client.AccountBalance)
    {
        cout << "Amount Exceeds The Balance , You Can Withdraw Up To : " << Client.AccountBalance << endl;
        cout << "Please Enter Another Amount ? ";
        cin >> WithdrawAmount;
    }
    DepositeFunction(AllClients, WithdrawAmount * -1, Account_Number);
}
void EachClientTotalBalance(stClient_Data Client)
{
    cout << "| " << left << setw(17) << Client.Account_Number;
    cout << "| " << left << setw(35) << Client.Name;
    cout << "| " << left << setw(10) << Client.AccountBalance;
}
void ShowTotalBalance()
{
    vector<stClient_Data> AllClients = LoadClientsFromFile();
    cout << "\n\t\t\t\tBalance List (" << AllClients.size() << ") Client(s).\n";
    cout << "________________________________________________________";
    cout << "____________________________________\n\n";
    cout << "| " << left << setw(17) << " Account Number";
    cout << "| " << left << setw(35) << " Client Name";
    cout << "| " << left << setw(10) << " Balance";
    cout << "\n________________________________________________________";
    cout << "____________________________________\n\n";
    double TotalBalances = 0;
    if (AllClients.size() == 0)
    {
        cout << "\n\nThere Is No Available Client In The System...\n";
    }
    else
    {
        for (stClient_Data& Client : AllClients)
        {
            EachClientTotalBalance(Client);
            TotalBalances += Client.AccountBalance;
            cout << endl;
        }
        cout << "________________________________________________________";
        cout << "____________________________________\n";
        cout << "\n\t\t\t\tTotal Balance = " << TotalBalances << endl;
    }

}
void PerformTransactionMenueOptions(EnTransaction YourTransactionMenueOption)
{
    switch (YourTransactionMenueOption)
    {
    case EnTransaction::Deposite:
    {
        system("cls");
        ShowDepositeScreen();
        GoBackToTransactionMenue();
        break;
    }
    case EnTransaction::Withdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionMenue();
        break;
    }
    case EnTransaction::Total_Balance:
    {
        system("cls");
        ShowTotalBalance();
        GoBackToTransactionMenue();
        break;
    }
    case EnTransaction::MainMenue:
    {
        ShowMainMenueScreen();
    }
    }
}
void PerformMainMenueOptions(EnMainMenueOptions YourChoice)
{
    switch (YourChoice)
    {
    case EnMainMenueOptions::eShowClientList:
    {
        system("cls");
        ShowClientList();
        GoBackToMainMenue();
        break;
    }
    case EnMainMenueOptions::eAddNewClient:
    {
        system("cls");
        ShowAddNewClientScreen();
        GoBackToMainMenue();
        break;
    }
    case EnMainMenueOptions::eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }
    case EnMainMenueOptions::eUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }
    case EnMainMenueOptions::eFindClient:
    {
        system("cls");
        FindClient();
        GoBackToMainMenue();
        break;
    }
    case EnMainMenueOptions::eTransactions:
    {
        system("cls");
        ShowTransactionMenueOptions();
        break;
    }
    case EnMainMenueOptions::eManageUsers:
    {
        system("cls");
        ShowManageUsersMenueOptions();
        break;
    }
    case EnMainMenueOptions::eLogout:
    {
        system("cls");
        Login();
        break;
    }
    }
}
void PerformUserMenueOptions(EnUserMenueOptions YourChoice)
{
    switch (YourChoice)
    {
    case EnUserMenueOptions::eShowUsersList:
    {
        system("cls");
        ShowUserList();
        GoBackToUserMainMenue();
        break;
    }
    case EnUserMenueOptions::eAddNewUser:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToUserMainMenue();
        break;
    }
    case EnUserMenueOptions::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToUserMainMenue();
        break;
    }
    case EnUserMenueOptions::eUpdateUser:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToUserMainMenue();
        break;
    }
    case EnUserMenueOptions::eFindUser:
    {
        system("cls");
        FindUser();
        GoBackToUserMainMenue();
        break;
    }
    case EnUserMenueOptions::eMainMenue:
    {
        system("cls");
        ShowMainMenueScreen();
        break;
    }
    }
}
void ShowTransactionMenueOptions()
{
    if (!CheckUserPermissions(UserPermissions::ETransactions))
    {
        ShowDeniedScreen();

        return;
    }
    system("cls");
    cout << "================================================\n";
    cout << "\t\tTransaction Menue Screen\n";
    cout << "================================================\n";
    cout << "\t[1] Deposite.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balance.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "================================================\n";
    PerformTransactionMenueOptions(EnTransaction(ReadTransactionMenueOptions()));
}
void ShowMainMenueScreen()
{
    system("cls");
    cout << "================================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "================================================\n";
    PerformMainMenueOptions(EnMainMenueOptions(ReadMainMenueOptions()));
}
void  ShowManageUsersMenueOptions()
{
    if (!CheckUserPermissions(UserPermissions::EManageUsers))
    {
        ShowDeniedScreen();

        return;
    }
    system("cls");
    cout << "================================================\n";
    cout << "\t\tUsers Menue Screen\n";
    cout << "================================================\n";
    cout << "\t[1] Show User List.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "================================================\n";
    PerformUserMenueOptions(EnUserMenueOptions(ReadUserMenueOptions()));
}
bool LoadLoginInfo(string UserName,string Password)
{
    vector<stUser_Data>AllUsers=LoadUsersFromFile();
    if (!FindUserByUserNameAndPassword(AllUsers, CurrentUser, UserName,Password))
    {
        return true;
    }
    return false;
}

void Login()
{
    bool LoginChoice = false;
    do
    {
        system("cls");
        string UserName,Password;
        cout << "================================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "================================================\n";
        if (LoginChoice)
        {
            cout << "Invalid UserName/Password -) : \n";
        }

        cout << "Enter Your Name Please : ";
        cin >> UserName;
        cout << "Enter Password Please : ";
        cin >> Password;
       LoginChoice= LoadLoginInfo(UserName,Password);

    } while (LoginChoice);
    ShowMainMenueScreen();
}
int main()
{
    Login();
    system("pause>0");
}

