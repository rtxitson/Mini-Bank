#include<bits/stdc++.h>
using namespace std;

enum AccountType { CURRENT, SAVINGS };

enum CurrencyType { DOLLER };

class Currency {
public:
    static int covertInCentsFromDollers(string );
    static string showCentsToDollers(int);
};
int Currency::covertInCentsFromDollers(string money) {
    int dPos = money.find('D');
    string doller = money.substr(0,dPos);
    int space = money.find(' ');
    int cPos = money.find('C');
    string cents = money.substr(space+1, cPos-space-1);
    //cout<<doller<<" "<<cents<<endl;
    // if(doller[0] == '-' || cents[0] == '-'){
    //     return -1;
    // }
    return stoi(doller)*100+stoi(cents);
}
string Currency::showCentsToDollers(int cents) {
    if(cents==0){
        return "0";
    }
    int dollers = cents / 100;
    cents%=100;

    return to_string(dollers)+"D "+to_string(cents)+"C";
}



// ***** USER ******
class User{
public:
    User(string,string,int);
    User();
    string getId();
private:
    string id;
    string name;
    string gender;
    int age;
    string getUniqueId();
};
User::User(string name,string gender,int age){
    this->id = getUniqueId();
    this->name = name;
    this->gender = gender;
    this->age = age;
}
User::User(){

}
string User::getId(){
    return this->id;
}
string User::getUniqueId(){
    static int userId = 0;
    userId++;
    return "U"+to_string(userId);
}

// ****** END *******


// ******* Account *******
class Account{
public:
    Account(User&);
    string getAccNo();
    string getUserId();
    bool updateBalance(int);
    int getBalance();
    CurrencyType getCurrencyType();
private:
    string accNo;
    string userId;
    User user;
    int type;
    int balance;
    CurrencyType currencyType;
    string getUniqueAccNo();
};

Account::Account(User& user) {
    this->accNo = getUniqueAccNo()+"-"+user.getId();
    this->userId = user.getId();
    this->user = user;
    this->type = AccountType::SAVINGS;
    this->currencyType = CurrencyType::DOLLER;
    this->balance = 0;
}
bool Account::updateBalance(int amount){
    if(abs(this->balance + amount) > 1e9){
        return false;
    }
    this->balance += amount;
    return true;
}
int Account::getBalance(){
    return this->balance;
}
string Account::getUserId(){
    return this->userId;
}
string Account::getAccNo(){
    return this->accNo;
}
CurrencyType Account::getCurrencyType(){
    return this->currencyType;
}
string Account::getUniqueAccNo(){
    static int accId = 0;
    accId++;
    return "A"+to_string(accId);
}

// ******* END *********

// ******* Bank ********
class Bank {
public:
    bool registerUser(User &);
    bool verifyUser(User &);
    string checkBalance(User &);
    bool credit(User &,string &);
    bool debit(User &,string &);
    Account& getAccountForUser(User &);
    // int parseDollarCents(string &);
    // string getBalanceInDollarCent(User &);
private:
    vector<User> users;
    vector<Account> accounts;

};
bool Bank::verifyUser(User &user){
    for(User registeredUser: this->users){
        if(registeredUser.getId() == user.getId()){
            return true;
        }
    }
    return false;
}
bool Bank::registerUser(User &user){
    if(verifyUser(user)){
        cout<<"User already registered\n";
        return false;
    }
    this->users.push_back(user);
    Account account = Account(user);
    this->accounts.push_back(account);
    cout<<"User Successfully registered\n"<<endl;
    return true;
}
Account& Bank::getAccountForUser(User& user){
    for(int i=0;i<accounts.size();i++){
        if(accounts[i].getUserId() == user.getId()){
            return accounts[i];
        }
    }
    
}
string Bank::checkBalance(User& user){
    if(!verifyUser(user)){
        cout<<"User is not registered\n"<<endl;
        return "-1";
    }
    Account &account = getAccountForUser(user);

    if(account.getCurrencyType() == CurrencyType::DOLLER){
        return Currency::showCentsToDollers(account.getBalance());
    }

    //cout<<"Balance is "<<account.getBalance()<<endl;
    return to_string(account.getBalance());
}
bool Bank::credit(User &user,string &money){
    if(!verifyUser(user)){
        cout<<"User is not registered\n"<<endl;
        return false;
    }

    Account &account = getAccountForUser(user);

    int amount = -1;

    if(account.getCurrencyType() == CurrencyType::DOLLER){
        amount = Currency::covertInCentsFromDollers(money);
    }

    if(amount==0){
        cout<<"Wrong Input\n";
        return false;
    }
    bool success = account.updateBalance(amount);
    if(!success){
        cout<<"Something went wrong!! try again later\n";
        return false;
    }
    cout<<"Credited!! Remaining Balance is "<<account.getBalance()<<endl;
    return true;
}

bool Bank::debit(User &user,string &money){
    if(!verifyUser(user)){
        cout<<"User is not registered\n"<<endl;
        return false;
    }
    Account &account = getAccountForUser(user);

    int amount = -1;

    if(account.getCurrencyType() == CurrencyType::DOLLER){
        amount = Currency::covertInCentsFromDollers(money);
    }

    if(amount==0){
        cout<<"Wrong Input\n";
        return false;
    }

    bool success = account.updateBalance(-1*amount);

    if(!success){
        cout<<"Something went wrong!! try again later\n";
        return false;
    }
    cout<<"Debited!! Remaining Balance is "<<account.getBalance()<<endl;
    return true;
}

// ******* END *******


int main(){
    int option;
    User user1 = User("Indranil","M",18);
    User user2 = User("Anagha","F",22);
    Bank bank;
    bank.registerUser(user1);
    bank.registerUser(user2);
    vector<User> users;
    users.push_back(user1);
    users.push_back(user2);


    int userInd = -1;

    do{
        cout<<"Choose User to be operated on and press -1 to exit from Bank-Server\n";
        cin>>userInd;

        do{
            string money = "";
            cout<<"1. Credit\n2. Debit\n3. Check Balance\n4. Exit\n";
            cin>>option;
            switch(option){
                case 1: 
                    cout<<"Enter Amount to be credited\n";
                    cin.ignore();
                    getline(cin,money); 
                    bank.credit(users[userInd],money);
                    break;
                case 2: 
                    cout<<"Enter Amount to be debited\n";
                    cin.ignore();
                    getline(cin,money); 
                    bank.debit(users[userInd], money);
                    break;
                case 3: 
                    cout<<"Current balance is - "<<bank.checkBalance(users[userInd])<<endl;
                    break;
                case 4:
                    cout<<"Thank You!!\n";
                    break;

            }

        }while(option!=4);


    }while(userInd!=-1);
        
    return 0;
}
