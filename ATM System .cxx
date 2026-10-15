#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

const string ClientFileName = "Clients1.txt";

enum enAtmMenu
{
	eQuickWithdrawal = 1,
	eNormalWithdrawal,
	eDeposit,
	eBalance,
	eLogout
};
struct stClient
{
	string Accountnumber;
	string Pincode;
	string Name;
	string Phone;
	double Accountbalance = 0;
	bool MarketForDelete = false;
};

stClient CurrentClient;

void ShowAtmMenuScreen();
void Login();

short ReadMenuNumber()
{
	short Number = 0;
	cout << "Please enter your choice :";
	cin >> Number;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		cout << "\n\nIs invalid number enter a number:";
		cin >> Number;
	}

	while (Number < 1 || Number > 5)
	{
		cout << "\nEnter choice between 1 and 5 :";
		cin >> Number;

		while (cin.fail())
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			cout << "\n\nIs invalid number enter a number:";
			cin >> Number;
		}
	}
	return Number;
}
vector<string> SplitString(string text, string delim)
{
	string Word;
	short Pos = 0;
	vector<string> vWord;
	while ((Pos = text.find(delim)) != std::string::npos)
	{
		Word = text.substr(0, Pos);
		if (Word != "")
			vWord.push_back(Word);

		text.erase(0, Pos + delim.length());
	}
	if (text != "")
		vWord.push_back(text);

	return vWord;
}
stClient ConvertClientLineToRecord(string Line, string Delim = "#//#")
{
	stClient client;
	vector<string> vWord;
	vWord = SplitString(Line, Delim);

	client.Accountnumber = vWord[0];
	client.Pincode = vWord[1];
	client.Name = vWord[2];
	client.Phone = vWord[3];
	client.Accountbalance = stod(vWord[4]);

	return client;
}
vector<stClient> LoadClientDataFromFile(string FileName)
{
	string Line;
	stClient Client;
	vector<stClient> vData;

	fstream file;
	file.open(FileName, ios::in);

	if (file.is_open())
	{
		while (getline(file, Line))
		{
			Client = ConvertClientLineToRecord(Line);
			vData.push_back(Client);
		}
		file.close();
	}
	return vData;
}
bool FoundClientByAccountNumAndPincode(string Accountnum, string Pincode, stClient &Current)
{
	vector<stClient> vClients = LoadClientDataFromFile(ClientFileName);

	for (stClient &C : vClients)
	{
		if (C.Accountnumber == Accountnum && C.Pincode == Pincode)
		{
			Current = C;
			return true;
		}
	}
	return false;
}
bool CheckAndLoadClientInfo(string Accountnum, string Pincode)
{
	if (FoundClientByAccountNumAndPincode(Accountnum, Pincode, CurrentClient))
	{
		return true;
	}
	return false;
}
string ConvertClientRecordToLine(stClient vClient, string Delim = "#//#")
{
	string Line;
	Line = vClient.Accountnumber + Delim;
	Line += vClient.Pincode + Delim;
	Line += vClient.Name + Delim;
	Line += vClient.Phone + Delim;
	Line += to_string(vClient.Accountbalance);
	return Line;
}
void LoadClientDataToFile(string Filename, vector<stClient> vUpdate)
{
	string Line;
	fstream file;
	file.open(Filename, ios::out);
	if (file.is_open())
	{
		for (stClient &c : vUpdate)
		{
			Line = ConvertClientRecordToLine(c);
			file << Line << endl;
		}
		file.close();
	}
}
void DepositClientByAccountnumber(string Accountnumber, double Amount, vector<stClient> &vData)
{
	char Answer = 'N';
	cout << "\n\nYour balance " << CurrentClient.Accountbalance;
	cout << ", are you sure about the withdrawal process enter [Y]/[N] :";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		for (stClient &C : vData)
		{
			if (C.Accountnumber == Accountnumber)
			{
				C.Accountbalance += Amount;
				LoadClientDataToFile(ClientFileName, vData);
				cout << "\nThe operation was successfully, your current balance (" << C.Accountbalance << ")";
				break;
			}
		}
	}
}
int GetQuickWithdrawalAmount(short Selection)
{
	switch (Selection)
	{
	case 1:
		return 50;
		break;
	case 2:
		return 100;
		break;
	case 3:
		return 500;
		break;
	case 4:
		return 1000;
		break;
	case 5:
		return 2000;
		break;
	case 6:
		return 5000;
		break;
	case 7:
		return 7000;
		break;
	case 8:
		return 10000;
		break;
	case 9:
		return 100000;
		break;
	case 10:
		return 200000;
		break;
	case 11:
		return 500000;
		break;
	case 12:
		return 1000000;
		break;
	default:
		return 0;
	}
}
void QuickWithdrawal(short AmountChoice)
{
	if(AmountChoice == 13)
	return;
	
	int WithdrawalAmount = GetQuickWithdrawalAmount(AmountChoice);

	if (WithdrawalAmount > CurrentClient.Accountbalance)
	{
		cout << "\nThe amount is greater than your current balance, enter another amount.\n";
		return;
	}

	vector<stClient> vData = LoadClientDataFromFile(ClientFileName);

	DepositClientByAccountnumber(CurrentClient.Accountnumber, WithdrawalAmount * -1, vData);

	CurrentClient.Accountbalance -= WithdrawalAmount;
}
short ReadQuickWithdrawalChoose()
{
	short Choice = 0;
	cout << "Enter choice between 1 and 13 :";
	cin >> Choice;

	while (Choice < 1 || Choice > 13)
	{
		cout << "Enter choice between 1 and 13 :";
		cin >> Choice;
	}

	return Choice;
}
void ShowQuickWithdrawalScreen()
{
	cout << "===============================\n";
	cout << "\tQuick Withdrawal.\n";
	cout << "===============================\n";

	printf("[1] %6d \t[2] %7d \t[3] %7d \t[4] %7d\n\n", 50, 100, 500, 1000);

	printf("[5] %6d \t[6] %7d \t[7] %7d \t[8] %7d\n\n", 2000, 5000, 7000, 10000);

	printf("[9] %d \t[10] %d \t[11] %d \t[12] %d\n\n", 100000, 200000, 500000, 1000000);

	printf("Exit [13]\n");

	cout << "===============================\n";

	QuickWithdrawal(ReadQuickWithdrawalChoose());
}
void GoBackToAtmMenu()
{
	cout << "\n\nPress any kye to back :" << endl;
	system("pause>0");
	ShowAtmMenuScreen();
}
void NormalWithdrawal(double Amount)
{
	if (Amount > CurrentClient.Accountbalance)
	{
		cout << "\nThe amount is greater than your current balance, enter another amount.\n";
		return;
	}

	vector <stClient> vData = LoadClientDataFromFile(ClientFileName);
	
	DepositClientByAccountnumber(CurrentClient.Accountnumber, Amount * -1, vData);
	
	CurrentClient.Accountbalance -= Amount;
}
double ReadNormalWithdrawaAmount()
{
	int Amount = 0;
	cout << "Enter an amount of 5 multiples :";
	cin >> Amount;
	
	while(Amount % 5 != 0)
	{
		cout << "Enter an amount of 5 multiples :";
	  cin >> Amount;
	}
	
	return Amount;
}
void ShowNormalWithdrawalScreen()
{
	cout << "================================\n";
	cout << "\tNormal Withdrawal.\n";
	cout << "================================\n";

	double TheAmount = ReadNormalWithdrawaAmount();

	NormalWithdrawal(TheAmount);
}
double ReadDepositAmount()
{
	double TheAmount = 0;
	cout << "Enter howmuch do you want to deposit :";
	cin >> TheAmount;
	
	while(TheAmount <= 0)
	{
	  cout << "Enter positiv amount :";
	  cin >> TheAmount;	
	}
	
	return TheAmount;
}
void Deposit()
{
	double TheAmount = ReadDepositAmount();
	
	vector <stClient> vData = LoadClientDataFromFile(ClientFileName);
	
	DepositClientByAccountnumber(CurrentClient.Accountnumber, TheAmount, vData);
	
	CurrentClient.Accountbalance += TheAmount;
}
void ShowDepositScreen()
{
	cout << "===============================\n";
	cout << "\tDeposit.\n";
	cout << "===============================\n";
	
  Deposit();
}
void ShowTotalBalanceScreen()
{
	cout << "\n\n———————————————————————————————————————————————————————————————————\n";
	cout << left;
	cout << "\t\t" << setw(15) << "Total balance "
		 << "|" << CurrentClient.Accountbalance;
	cout << "\n———————————————————————————————————————————————————————————————————\n";
}
void PerformAtmMenu(enAtmMenu Selection)
{
	switch (Selection)
	{
	case eQuickWithdrawal:
		system("cls");
		ShowQuickWithdrawalScreen();
		GoBackToAtmMenu();
		break;
	case eNormalWithdrawal:
		system("cls");
		ShowNormalWithdrawalScreen();
		GoBackToAtmMenu();
		break;
	case eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToAtmMenu();
		break;
	case eBalance:
		system("cls");
		ShowTotalBalanceScreen();
		GoBackToAtmMenu();
		break;
	case eLogout:
		Login();
		break;
	}
}
void ShowAtmMenuScreen()
{
	system("cls");
	cout << "================================\n";
	cout << "\t   ATM MENU\n";
	cout << "================================\n";
	cout << "For quick withdrawal enter  [1]:\n";
	cout << "For normal withdrawal enter [2]:\n";
	cout << "For deposit enter           [3]:\n";
	cout << "For total balance enter     [4]:\n";
	cout << "Logout                      [5]:\n";
	cout << "================================\n";

	PerformAtmMenu((enAtmMenu)ReadMenuNumber());
}
void Login()
{
	system("cls");
	cout << "==============================\n";
	cout << "\tLogin Screen.\n";
	cout << "==============================\n";

	string Accountnumber, Pincode;
	bool IsLoginFailed = false;

	do
	{
		if (IsLoginFailed)
			cout << "\nInvalid account number/pin code\n";

		cout << "Enter account number :";
		getline(cin >> ws, Accountnumber);
		cout << "Enter pin code       :";
		getline(cin, Pincode);

		IsLoginFailed = !CheckAndLoadClientInfo(Accountnumber, Pincode);

	} while (IsLoginFailed);
	ShowAtmMenuScreen();
}
int main()
{
	Login();
	return 0;
}