#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>  

using namespace std;
string encrypt(string);
string decrypt(string);
class User {
    string name, password;
public:
    User(string n = "", string p = "") {
        name = n;
        password = p;
    }

    string getName() {
        return name;
    }

    void saveToFile(fstream &outFile) { // fstream ka object &outFile. jo naya nhi benga
    	
        outFile << name << endl << encrypt(password) << endl;

    }

    void loadFromFile(fstream &inFile) {  // infile object . all ready bna hai agey
        getline(inFile, name);
        getline(inFile, password);
        password = decrypt(password);
    }
};

class Account {
private:
    string filename;
    bool savings;  //ture for saving or false
protected:
    string starter = "PK-345789";
    int number;
    double balance = 0;
    double amount = 0;
    User owner;            //name or passward chachye wo user class se ayega
public:
    Account(bool savings, int number = 1, double balance = 0.0, User owner = User()) : savings(savings), number(number), balance(balance), owner(owner) {
        stringstream ss;   //integer ko string mai convert krne k leye
        ss << number; 

        string file = "account_PK-345789" + ss.str() + ".txt";
        filename = file; 
    }

    virtual void deposit(double amount) = 0;

    virtual void withdraw(double amount) {
        if (amount > 0 && amount <= balance) balance -= amount;  //withdraw 
    }

    virtual double newBalance() {
        return balance + amount;     // deposit
    }

    virtual string getAccountType() {  // 
        if (savings) return "Savings";
        return "Current";
    }

    void displayAccountDetails() {
    	system("cls");
    	system("color F0"); //font black , bg white
    	
        cout << "\n\n***********************************************************\n";
        
        cout << "                  DISPLAYING ACCOUNT DETAILS                \n";
        
        cout << "***********************************************************\n\n";
        cout << "\t\t" << "Account Number: " << starter << number << endl;
        cout << "\t\t" << "Owner: " << owner.getName() << endl;
        cout << "\t\t" << "Balance: " << balance << endl;
        cout << "\t\t" << "Account Type: " << getAccountType() << endl;
        cout << "\n***********************************************************\n\n";
    }

    void saveToFile() {
        fstream outFile(filename.c_str(), ios::out);   //write 
        if (outFile.is_open()) {  //buildin function 
            cout << endl << "\t\tSaving updates to file " << filename.c_str() << endl;
            owner.saveToFile(outFile);  
            outFile << getAccountType() << endl << number << endl << balance << endl;
            outFile.close();
        } else {
            cout << "\t\tSomething Went Wrong" << endl
                << "File does not opened" << endl;
            exit(1);
        }
    }

    void loadFromFile(int accountNumber) {
        string type;
        fstream inFile(filename.c_str(), ios::in);  
        if (inFile.is_open()) {
            owner.loadFromFile(inFile);
            inFile >> type >> number >> balance;
            inFile.close();
        }
    }

    void deleteAccount() {
        remove(filename.c_str());
    }
};

class SavingsAccount : public Account {
public:
    SavingsAccount(int number = 0, double balance = 0.0, User owner = User()) : Account(true, number, balance, owner) {}

    double newBalance() override {    
        return balance + amount * 1.01;
    }

    void deposit(double newAmount) override {
        if (newAmount > 0) {
        	system("cls");
        	system("color 0A"); //bg black font green
            amount = newAmount;
            balance = newBalance();
            cout << "************************************************************\n\n";
            
            cout << "            Successfully Deposited PKR-" << amount << " \n";
            cout << "            PKR-" << amount * 0.01 << " Interest included \n";
             
            cout << "\n************************************************************\n\n";
        } else cout << "Invalid amount";
    }

    void withdraw(double amount) override {
        if (amount <= (balance + 20)) {
        	system("cls");
        	system("color F4"); 
            balance = balance - (amount + 20);
            cout << "************************************************************\n\n";
            
            cout << "        Successfully withdrawn PKR-" << amount << " \n";
            cout << "        PKR-20 Fee has been Deducted \n";
           
            cout << "\n************************************************************\n\n";
        } else cout << "Invalid Amount";
    }

    string getAccountType()  override {
        return "Savings";
    }
};

class CurrentAccount : public Account {
public:
    CurrentAccount(int number = 0, double balance = 0.0, User owner = User()) : Account(false,number, balance, owner) {}

    double newBalance() override {
        return balance + amount;
    }

    void deposit(double newAmount) override {
        if (newAmount > 0) {
        	system("cls");
        	system("color 0A");  
            amount = newAmount;
            balance = newBalance();
            cout << "************************************************************\n\n";
            cout << "           Successfully Deposited PKR-" << amount << " \n";
            cout << "           No Interest has been included \n";
            cout << "\n************************************************************\n\n";
        } else cout << "Invalid amount";
    }

    void withdraw(double amount) override {
        if (amount <= balance) {
        	system("cls");
        	system("color F4"); 
            balance = balance - amount;
            cout << "************************************************************\n\n";
            cout << "         Successfully withdrawn PKR-" << amount << " \n";
            cout << "         No tax has been Deducted \n";
            cout << "\n************************************************************\n";
        } else cout << "Invalid Amount";
    }

    string getAccountType()  override {
        return "Current";
    }
};

bool isValidPassword(string password) { //function
//	return true;
    if (password.length() < 8) return false;
    bool hasLetter = false, hasDigit = false;
    for (int i = 0; i < password.length(); i++) {
        if (isalpha(password[i])) hasLetter = true;  //buildin 
        if (isdigit(password[i])) hasDigit = true;
        if (hasLetter && hasDigit) return true;
    }
    return false;
}

void createAccount() {
    const string starter = "PK-345789";
    string name, password;
    int accountNumber = 1, accountType;
    double initialDeposit;
    while (true) {
        stringstream ss;
        ss << accountNumber;
        string filename = "account_PK-345789" + ss.str() + ".txt";
        ifstream inFile(filename.c_str());
//        cout<<filename<<" is opening moving to next ";
        if (!inFile.is_open()) break;
//        else cout<<inFile<< " file is opened";
        
        ++accountNumber;
    }

    cout << "\tEnter name: ";
    cin.ignore();
    getline(cin, name);
    cout << "\tEnter password: ";
    do {
        getline(cin, password);
        if (!isValidPassword(password)) {
            cout << "\tPassword must be at least 8 characters with letters and numbers\n";
            cout << "\tPlease Try Again" << endl << endl;
        }
    } while (!isValidPassword(password));

    cout << "\tEnter initial deposit: ";
    cin >> initialDeposit;
    cout << "\tEnter account type (1 for Savings, 2 for Current): ";
    while (cin >> accountType && (accountType < 1 || accountType > 2)) {
        cout << "\tWrong Choice Enter Again" << endl;
    }

    User newUser(name, password);
    Account* account = NULL;
    
    if (accountType == 1) {
    	SavingsAccount savingsAccount(accountNumber, initialDeposit * 1.01, newUser);
        account = &savingsAccount;

    } else {
    	CurrentAccount currentAccount(accountNumber, initialDeposit, newUser);
        account = &currentAccount;
    }
    account->saveToFile();

    cout << endl << "\t\tAccount created successfully.\n\t\tYour Account Number is " << starter << accountNumber << endl<<endl;
}

bool login(string name, string password, Account* &account) {
    for (int i = 1; i <= 1000; ++i) {
        stringstream ss;
        ss << i;
        string filename = "account_PK-345789" + ss.str() + ".txt";
        ifstream inFile(filename.c_str());
        if (inFile.is_open()) {
            string file_name, file_password, file_accountType;
            getline(inFile, file_name);
            getline(inFile, file_password);
            
            file_password = decrypt(file_password);
            
            
            getline(inFile, file_accountType);
            if (file_name == name && file_password == password) {// pure virtual fuction ki wajah se direct access 
            // nhi kr skhtai. is leye donon acc k refrence aa rhe hoo 
                if (file_accountType == "Savings") {
                    account = new SavingsAccount(i);
                } else if (file_accountType == "Current") {
                    account = new CurrentAccount(i);
                }

                else {
                    cout << "Account type not found";
                }
                account->loadFromFile(i);
                return true;
            }
        }
    }
    return false;
}

int main() {
    int choice;
    Account* instantAccount = NULL;
    cout <<endl <<endl<< "\t\t" << "***********************************************************\n";
       	 cout << "\t\t" << "\n\n";

    cout << "\t\t" << "   |---------------------------------------|          \n";
    cout << "\t\t" << "   |________                      _________|          \n";
    cout << "\t\t" << "            |   WELCOME TO ZAM    |                   \n";
    cout << "\t\t" << "            |   BANKING SYSTEM    |                   \n";
    cout << "\t\t" << "            |---------------------|                   \n";
    cout << "\t\t" << "            |  1. Login           |                   \n";
    cout << "\t\t" << "            |  2. Create Account  |                   \n";
    cout << "\t\t" << "            '----------------------'                   \n";
    cout << "\t\t" << "***********************************************************\n\n";
    cout << "\t\t" << "Enter choice => ";
    cin >> choice;

    if (choice == 1) {
        while (true) {
            login:
            string name, password;
            cout << "\t\t" << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "\t\t" << "Enter password: ";
            getline(cin, password);
  
            if (login(name, password, instantAccount)) {
                cout << "\t\tLogin successful!" << endl;
                int option;
                do {
                    cout << "\n\t\t1. Deposit\n\t\t2. Withdraw\n\t\t3. Display Account Details\n\t\t4. Delete Account\n\t\t5. Exit\n\t\tEnter choice: ";
                    cin >> option;
                    if (option == 1) {
                        double amount;
                        cout << "\t\t" << "Enter amount to deposit: ";
                        cin >> amount;
                        instantAccount->deposit(amount);
                    } else if (option == 2) {
                        double amount;
                        cout << "\t\t" << "Enter amount to withdraw: ";
                        cin >> amount;
                        instantAccount->withdraw(amount);
                    } else if (option == 3) {
                        instantAccount->displayAccountDetails();
                    }
                    else if (option == 4) {
                        instantAccount->deleteAccount();
                        cout << "\t\t"<<"Account deleted";
                        exit(0);
                    }
                    else if (option == 5) {
                        
                        cout << "\t\t"<<"Exiting ...";
                        exit(0);
                    }
                    instantAccount->saveToFile();
                } while (option < 5);
                
                delete instantAccount;
                break;
            } else {
            	system("cls");
                cout << "\t\t" << "Login failed. Incorrect name or password.\n\n\t\t Do you want to create an account?" << endl;
                main();
            }
        }
    } else if (choice == 2) {
        createAccount();
        cout << "\t\t" << "For logging in, "<<endl;
        goto login;
    }
    return 0;
}

string encrypt(string s){
    string encrypted = "";   //strat mai khalii hoga
    char c;
    int length = s.length();
	for (int i = 0 ; i <length; i++){
		int n = s[i];
		n++;
		c = n;
		encrypted += c;
	}

	return encrypted;
}
string decrypt(string s){
	char c;
	int length = s.length();
    string encrypted = "";
	for (int i = 0 ; i <length; i++){
		int n = s[i];
		n--;
		c = n;
		encrypted += c;
	}
//	cout<<encrypted;
	return encrypted;
}

