#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>

using namespace std;


//classes used 

class account
{
	int accountNum;
	char name[50];
	int deposit;
	char type;
public:
	void createAccount(); //get data
	void showAccount() const; //show data
	void modify(); //add new data
	void dep(int); // add to balance amount
	void draw(int); // subtract from balance amount
	void report() const; // show data
	int returnAccountNum() const; // return Account Number
	int returnBalance() const; // return Account balance
	char returnType() const; // return type of account
};        // class

void account::createAccount()
{
	cout << "\n Please enter the account number: ";
	cin >> accountNum;
	cout << "\n\n Enter the Name of the account holder: ";
	cin.ignore();
	cin.getline(name, 50);
	cout << "\nEnter Type of account (C/S): ";
	cin >> type;
	type = toupper(type);
	cout << "\n Enter the Initial amount(>= 500 for Savings and >= 1000 for current):";
	cin >> deposit;
	cout << "\n\n\nAccount Created..";
}

void account::showAccount() const
{
	cout << "\nAccount No. : " << accountNum;
	cout << "\nAccount Holder Name : ";
	cout << name;
	cout << "\nType of Account : " << type;
	cout << "\nBalance amount : " << deposit;
}

void account::modify()
{
	cout << "\nAccount Number: " << accountNum;
	cout << "\nModify Account Holder Name: ";
	cin.ignore();
	cin.getline(name, 100);
	cout << "\nModify Type of Account: ";
	cin >> type;
	type = toupper(type);
	cout << "\nModify Balance amount: ";
	cin >> deposit;
}

void account::dep(int x)
{
	deposit += x;
}

void account::draw(int x)
{
	deposit -= x;
}

void account::report() const
{
	cout << accountNum << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;
}

int account::returnAccountNum() const
{
	return accountNum;
}

int account::returnBalance() const
{
	return deposit;
}

char account::returnType() const
{
	return type;
}

// declareing functions 

void writeAccount();
void displayDetails(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAll();
void depositWithdraw(int, int);
void intro();

void writeAccount() //write in file
{
	account ac;
	ofstream outFile;
	outFile.open("account.dat", ios::binary | ios::app);
	ac.createAccount();
	outFile.write(reinterpret_cast<char*> (&ac), sizeof(account));
	outFile.close();
}

void displayDetails(int n)
{
	account ac;
	bool flag = false;
	ifstream inFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "File could not be open!! Press any key...";
		return;
	}
	cout << "\nBalanace Details\n";

	while (inFile.read(reinterpret_cast<char*> (&ac), sizeof(account)))
	{
		if (ac.returnAccountNum() == n)
		{
			ac.showAccount();
			flag = true;
		}
	}
	inFile.close();
	if (flag == false)
		cout << "\n\nAccount number does not exist";
}

void modifyAccount(int n)
{
	bool found = false;
	account ac;
	fstream File;
	File.open("account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "File could not be opened!! Press any Key...";
		return;
	}
	while (!File.eof() && found == false)
	{
		File.read(reinterpret_cast<char*> (&ac), sizeof(account));
		if (ac.returnAccountNum() == n)
		{
			ac.showAccount();
			cout << "\n\nEnter the New Details of Account" << endl;
			ac.modify();
			int pos = (-1) * static_cast<int>(sizeof(account));
			File.seekp(pos, ios::cur);
			File.write(reinterpret_cast<char*> (&ac), sizeof(account));
			cout << "\n\n\t Record Updated";
			found = true;
		}
	}
	File.close();
	if (found == false)
		cout << "\n\n Record Not Found ";
}

void deleteAccount(int n)
{
	account ac;
	ifstream inFile;
	ofstream outFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "File could not be open !! Press any Key...";
		return;
	}
	outFile.open("Temp.dat", ios::binary);
	inFile.seekg(0, ios::beg);
	while (inFile.read(reinterpret_cast<char*> (&ac), sizeof(account)))
	{
		if (ac.returnAccountNum() != n)
		{
			outFile.write(reinterpret_cast<char*> (&ac), sizeof(account));
		}
	}
	inFile.close();
	outFile.close();
	remove("account.dat");
	rename("Temp.dat", "account.dat");
	cout << "\n\n\tRecord Deleted ..";
}

void displayAll()
{
	account ac;
	ifstream inFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "File could not be open !! Press any Key...";
		return;
	}
	cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
	cout << "====================================================\n";
	cout << "A/c no.      NAME           Type  Balance\n";
	cout << "====================================================\n";
	while (inFile.read(reinterpret_cast<char*> (&ac), sizeof(account)))
	{
		ac.report();
	}
	inFile.close();
}

void depositWithdraw(int n, int option)
{
	int amt;
	bool found = false;
	account ac;
	fstream File;
	File.open("account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "File could not be open !! Press any Key...";
		return;
	}
	while (!File.eof() && found == false)
	{
		File.read(reinterpret_cast<char*> (&ac), sizeof(account));
		if (ac.returnAccountNum() == n)
		{
			ac.showAccount();
			if (option == 1)
			{
				cout << "\n\n\tTO DEPOSITE AMOUNT ";
				cout << "\n\nEnter The amount to be deposited";
				cin >> amt;
				ac.dep(amt);
			}
			if (option == 2)
			{
				cout << "\n\n\tTO WITHDRAW AMOUNT ";
				cout << "\n\nEnter The amount to be withdraw";
				cin >> amt;
				int bal = ac.returnBalance() - amt;
				if ((bal < 500 && ac.returnType() == 'S') || (bal < 1000 && ac.returnType() == 'C'))
					cout << "Insufficience balance";
				else
					ac.draw(amt);
			}
			int pos = (-1) * static_cast<int>(sizeof(ac));
			File.seekp(pos, ios::cur);
			File.write(reinterpret_cast<char*> (&ac), sizeof(account));
			cout << "\n\n\t Record Updated";
			found = true;
		}
	}
	File.close();
	if (found == false)
		cout << "\n\n Record Not Found ";
}

void intro()
{
	cout << "\n\n\n\t  BANK";
	cout << "\n\n\tMANAGEMENT";
	cout << "\n\n\t  SYSTEM";
	cin.get();
}

int main()
{
	char ch;
	int num;
	intro();
	do
	{
		system("cls");
		cout << "\n\n\n\tMAIN MENU";
		cout << "\n\n\t01. NEW ACCOUNT";
		cout << "\n\n\t02. DEPOSIT AMOUNT";
		cout << "\n\n\t03. WITHDRAW AMOUNT";
		cout << "\n\n\t04. BALANCE ENQUIRY";
		cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
		cout << "\n\n\t06. CLOSE AN ACCOUNT";
		cout << "\n\n\t07. MODIFY AN ACCOUNT";
		cout << "\n\n\t08. EXIT";
		cout << "\n\n\tSelect Your Option (1-8) ";
		cin >> ch;
		system("cls");
		switch (ch)
		{
		case '1':
			writeAccount();
			break;
		case '2':
			cout << "\n\n\tEnter The account No. : "; cin >> num;
			depositWithdraw(num, 1);
			break;
		case '3':
			cout << "\n\n\tEnter The account No. : "; cin >> num;
			depositWithdraw(num, 2);
			break;
		case '4':
			cout << "\n\n\tEnter The account No. : "; cin >> num;
			displayDetails(num);
			break;
		case '5':
			displayAll();
			break;
		case '6':
			cout << "\n\n\tEnter The account No. : "; cin >> num;
			deleteAccount(num);
			break;
		case '7':
			cout << "\n\n\tEnter The account No. : "; cin >> num;
			modifyAccount(num);
			break;
		case '8':
			cout << "\n\n\tThanks for using bank managemnt system";
			break;
		default:cout << "\a";
		}
		cin.ignore();
		cin.get();
	} while (ch != '8');
	return 0;
}

