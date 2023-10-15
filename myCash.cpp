
#include<bits/stdc++.h>
using namespace std;

int user_count = -1;
int current_user_idx = -1;
int trnx_count = 0;


bool verifyOTP();
void update_user_file(string _phone, string _name, string _pin, string _balance , string _status);

class Member{
    private:
        string phone;
        string name;
        string pin;
        int balance;
        int is_active;

    
    public:
        bool logged_in = false;
        
        string getPhone(){
            return phone;
        }
        string getPin(){
            return pin;
        }
        int getBalance(){
            return balance;
        }
        string getNmae(){
            return name;
        }
        int getStatus(){
            return is_active;
        }

        void transaction(int amount){
            balance  += amount;
        }
        void setMember(string _phone, string _name,string _pin, int _balance, int _status){
            phone = _phone;
            name = _name;
            pin = _pin;
            balance = _balance;
            is_active = _status;
            ofstream w_user_file;
            w_user_file.open("users.txt", ios_base::app);
            string str = "\n";
            str += phone;
            str += " ";
            str += name;
            str += " ";
            str += pin;
            str += " ";
            str += to_string(balance);
            str += " ";
            str += to_string(is_active);
            w_user_file << str;
            w_user_file.close();
        }

        void restoreMember(string _phone, string _name,string _pin, int _balance, int _status){
            phone = _phone;
            name = _name;
            pin = _pin;
            balance = _balance;
            is_active = _status;
        }
        
        bool checkStatus(){
            return is_active;
        }

        void printMember(){
            cout << "Phone: " << phone << "\n";
            cout << "Name: " << name << "\n";
            cout << "PIN: " << pin << "\n";
            cout << "Balance : " << balance << "\n";
            cout << "status : " << is_active << "\n";
        }

        void checkBalance(){
            cout << "Current Balance: " << balance << "\n";
        }

        void logout(){
            // cout << "logout" << "\n";
            logged_in = false;
            current_user_idx = -1;
            cout << "Successfully Logged out.\n";

        }

        void update(){
            char op;
            cout << "Do you want to update name? :_ ";
            cin >> op;
            if( op == 'y'){
                cout << "Name: ";
                cin >> name;
                cout << "Name successfully changed.\n";
            }
            cout << "Do you want to update pin? :_ ";
            cin >> op;
            if( op == 'y'){
                string npin, npin2;
                cout << "New pin: ";
                cin >> npin;
                cout << "Retype new pin: ";
                cin >> npin2;
                if( npin == npin2 ){
                    bool otp_verify = verifyOTP();
                    if( otp_verify){
                        pin = npin;
                        cout << "Pin successfully updated.\n";
                    }
                    else{
                        cout << "Incorrect OTP.\n";
                    }
                }
                else{
                    cout << "Pin doesn't match.\n";
                }
                
            }
            update_user_file(phone,name, pin, to_string(balance), to_string(is_active));
        }

        bool remove(){
            string upin;
            cout << "Enter your pin: ";
            cin >> upin;
            if( pin == upin){
                bool otp_verify = verifyOTP();
                if(otp_verify){
                    is_active = 0;
                    update_user_file(phone,name, pin, to_string(balance), to_string(is_active));
                    return true;
                }
                else{
                    cout << "OTP doesn't match.\n";
                }
            }
            else{
                cout << "Pin doesn't match.\n";
            }

            return false;

        }
        
};

class History{
    private:
         string user;
         string trnx_id;
         string trnx_type;
         string amount;
         string balance;
    public:
        void set_history(string _user, int _id, string type, string _amount,string _balance){
            user = _user;
            trnx_id = to_string(_id);
            trnx_type = type;
            amount = _amount;
            balance = _balance;
            ofstream w_history_file;
            w_history_file.open("history.txt", ios_base::app);
            string str = "\n";
            str += user;
            str += " ";
            str += trnx_id;
            str += " ";
            str += trnx_type;
            str += " ";
            str += amount;
            str += " ";
            str += balance;
            w_history_file << str;
            w_history_file.close();
        }
        void show_history(string user){
            cout << setw(8) << left << "Tran ID"<< setw(15) << left << "Description" << setw(8)  << left << "Amount" << setw(8) << left << "Balance\n";
            string _user, _id, type, _amount, _balance;
            ifstream r_history_file;
            r_history_file.open("history.txt");
            while(r_history_file.eof() == 0){
                r_history_file >> _user;
                r_history_file >> _id;
                r_history_file >> type;
                r_history_file >> _amount;
                r_history_file >>  _balance;
                if( user == _user){
                    cout << setw(8) << left << _id << setw(15) << left << type << setw(8)  << left << _amount << setw(8) << left << _balance << "\n";
                }

            }
            r_history_file.close();
            cout << "\n";
        }
};

History hstry;

Member users[1000];

void update_user_file(string _phone, string _name, string _pin, string _balance , string _status){
    string data = "";
    string phone, name, pin, balance, status;
    ifstream r_history_file;
    r_history_file.open("users.txt");
    int i = 0;
    while(r_history_file.eof() == 0){
        r_history_file >> phone;
        r_history_file >> name;
        r_history_file >> pin;
        r_history_file >> balance;
        r_history_file >> status;
        if( phone == _phone){
            pin = _pin;
            name = _name;
            balance = _balance;
            status = _status;
        }
        string temp = phone;
        temp += " ";
        temp += name;
        temp += " ";
        temp += pin;
        temp += " ";
        temp += balance;
        temp += " ";
        temp += status;
        if( i != 0){
            data +=  "\n";
        }
        data += temp;
        i++;
    }
    r_history_file.close();
    ofstream w_history_file;
    w_history_file.open("users.txt");
    w_history_file << data;
    w_history_file.close();


}


int searchMember(string phone){
    for(int i = 0; i <= user_count; i++){
        if(users[i].getPhone() == phone && users[i].checkStatus()){
            return i;
        }

    }
    return -1;
}

bool verifyOTP(){
    srand(time(0));
    int lb = 1000;
    int ub = 10000;
    int otp = rand() % (ub - lb + 1) + lb;
    cout << "myCash OTP: "<< otp << "\n";
    cout << "Enter OTP: ";
    int otp2;
    cin >> otp2;
    if( otp == otp2){
        return true;
    }

    return false;
}

int getPin(){
    string pass = "";
    char ch;
    ch = getchar();
    while(ch != 13){
        // getchar => getch
        pass.push_back(ch);
        cout << "*";
        ch = getchar();
    }
    cout << "\n";
    return (stoi(pass));
}

void write_trnx_count(){
    ofstream file;
    file.open("trnx.txt");
    file << to_string(trnx_count);
    file.close();
}

void sendMoney(){
    cout << "Enter recepient's number: ";
    string to, from;
    from = users[current_user_idx].getPhone(); 
    cin >> to;
    int amount;
    cout << "Enter amount: ";
    cin >> amount;
    int to_idx = searchMember(to);
    if( amount > users[current_user_idx].getBalance()){
        cout << "INSUFFICIENT BALANCE\n";
    }
    else if ( amount == 0){
        cout << "Amount can't be 0\n";
    }
    else if ( to_idx == -1){
        cout << "No user with that number\n";
    }
    else{
        users[current_user_idx].transaction(-1 * amount);
        users[to_idx].transaction(amount);
        hstry.set_history(from, ++trnx_count, "Send_Money", to_string(amount), to_string(users[current_user_idx].getBalance()));
        hstry.set_history(to, trnx_count, "Receive_Money", to_string(amount), to_string(users[to_idx].getBalance()));
        cout << "Send Money successful.\n";

        update_user_file(from, users[current_user_idx].getNmae(), users[current_user_idx].getPin(), to_string(users[current_user_idx].getBalance()), to_string(users[current_user_idx].getStatus()));
    
        update_user_file(to, users[to_idx].getNmae(), users[to_idx].getPin(), to_string(users[to_idx].getBalance()), to_string(users[to_idx].getStatus()));

    }
    write_trnx_count();
}
void cashIn(){
    int amount;
    cout << "Enter amount: ";
    cin >> amount;
    cout <<"Cash In " << amount << "\n";
    cout << "Are you sure(Y/N)?: ";
    char op;
    cin >> op;
    if( op == 'Y'){
        users[current_user_idx].transaction(amount);
        hstry.set_history(users[current_user_idx].getPhone(), ++trnx_count, "Cash-In", to_string(amount),to_string(users[current_user_idx].getBalance()));
        cout << "Cash in Successful.\n";

        update_user_file(users[current_user_idx].getPhone(), users[current_user_idx].getNmae(), users[current_user_idx].getPin(), to_string(users[current_user_idx].getBalance()), to_string(users[current_user_idx].getStatus()));
        write_trnx_count();

    }
    else{
        cout << "Cancelled by user\n";
    }
}
void cashOut(){
    int amount;
    cout << "Enter amount: ";
    cin >> amount;
    cout << "Cash Out " << amount << "\n";
    cout << "Are you sure(Y/N)? :";
    char op;
    cin >> op;
    if( op == 'Y'){
        bool otp_verified = verifyOTP();
        if(otp_verified){
            if( amount > users[current_user_idx].getBalance()){
                cout << "Insufficient Balance.\n";
            }
            else{
                users[current_user_idx].transaction(-1 * amount);
                hstry.set_history(users[current_user_idx].getPhone(), ++trnx_count,"Cash-Out", to_string(amount), to_string(users[current_user_idx].getBalance()));
                cout << "Cash Out successful.\n";
                update_user_file(users[current_user_idx].getPhone(), users[current_user_idx].getNmae(), users[current_user_idx].getPin(), to_string(users[current_user_idx].getBalance()), to_string(users[current_user_idx].getStatus()));
                write_trnx_count();
            }
        }
        else{
            cout << "OTP verification failed.\n";
        }
    }
    else{
        cout << "Cancelled by user.\n";
    }
}

void payBill(){
    cout << "pay bill" << "\n";
    cout << "Enter Bill Type (Gas/Electricity/Water/Internet-1/2/3/4): ";
    int op; cin >> op;
    int amount = 0;
    if(op == 1){
        cout << "Your Gas Bill: 1300\n";
        amount = 1300;
    }
    else if ( op == 2){
        cout << "Your Electricity Bill: 900\n";
        amount = 900;
    }
    else if ( op == 3){
        cout << "Your Water Bill: 500\n";
        amount = 500;
    }
    else if( op == 4){
        cout << "Your Internet Bill: 1000\n";
        amount = 1000;
    }
    else{
        cout << "Invalid Choice.";
        return;
    }
    cout << "Want to pay(Y/N)? :";
    char c; cin >> c;
    if( c == 'Y'){
        bool otp_verified = verifyOTP();
        if( otp_verified){
            if( amount > users[current_user_idx].getBalance()){
                cout << "Insufficient Balance.\n";
            }
            else{
                users[current_user_idx].transaction(-1 * amount);
                hstry.set_history(users[current_user_idx].getPhone(), ++trnx_count, "Bill_Pay", to_string(amount), to_string(users[current_user_idx].getBalance()));
                write_trnx_count();
                update_user_file(users[current_user_idx].getPhone(), users[current_user_idx].getNmae(), users[current_user_idx].getPin(), to_string(users[current_user_idx].getBalance()), to_string(users[current_user_idx].getStatus()));
                cout << "Bill Payment is Successful.\n";
            }
        }
        else{
            cout << "OTP verification failed.\n";
        }
    }
    else{
        cout << "Cancelled by user.\n";
    }
        
}
void history(){
    cout << "history" << "\n";
    string phone = users[current_user_idx].getPhone();
    hstry.show_history(phone);
}



void Register(){
    // cout << "\nregister here\n";
    string phone, pin, pin2, name;
    cout << "Phone: ";
    cin >> phone;
    int idx = searchMember(phone);
    if( idx == -1){
        cout << "Enter name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);
        cout << "Enter pin: ";
        cin >> pin;
        cout << "Reconfirm pin: ";
        cin >> pin2;
        if(pin == pin2){
            bool otp_verity = verifyOTP();
            if(otp_verity == true){
                user_count++;
                users[user_count].setMember(phone, name, pin, 0, 1);
                cout << "Registration completed.\n";


            }
            else{
                cout << "OTP doesn't match. Try Again.\n";
                Register();
            }
        }
        else{
            cout << "Pin doesn't match. Try again.\n";
            Register();
        }

    }
    else{
        cout << "Another user with this phone number exists. User another phone number or log in . \n";
    }

    
}

// MENU
void mainMenu(){
    // cout << "\nMENU HERE\n";
    cout << "********** MyCash Menu ********" << "\n";
    cout << "1. Update Me" << "\n";
    cout << "2. Remove Me" << "\n";
    cout << "3. Send Money" << "\n";
    cout << "4. Cash-in" << "\n";
    cout << "5. Cash-out" << "\n";
    cout << "6. Pay Bill" << "\n";
    cout << "7. Check Balance" << "\n";
    cout << "8. History" << "\n";
    cout << "9. Logout" << "\n";
    cout << "Enter Your Option (1-9):_" << "\n"; 
    int op;
    cin >> op;
    if( op == 1){
        users[current_user_idx].update();
    }
    else if ( op == 2){
        bool removed = users[current_user_idx].remove();
        if (removed){
            users[current_user_idx].logout();
            return;
        }
    }
    else if ( op == 3){
        sendMoney();

    }
    else if (op == 4){
        cashIn();
    }
    else if ( op == 5){
        cashOut();
    }
    else if ( op == 6){
        payBill();
    }
    else if (op == 7){
        users[current_user_idx].checkBalance();
    }
    else if ( op == 8){
        history();
    }
    else if ( op == 9){
        users[current_user_idx].logout();
        return;
    }
    else{
        cout << "Enter valid option.\n";
    }
    mainMenu();
    return;
}


bool loginMenu(){
    string phone, pin;
    cout << "Phone: ";
    cin >> phone;
    int idx = searchMember(phone);
    if( idx == -1){
        cout << "No user found\n";
    }
    else{
        string pin;
        cout << "Enter pin for account: " << phone << ":\n";
        cin >> pin;
        cout << "user pin : " << users[idx].getPin() << "\n";
        if( pin == users[idx].getPin()){
            users[idx].logged_in = true;
            current_user_idx = idx;
            return true;
        }
        else{
            cout << "Wrong pin. Try again.\n";
        }
    }

    return false;
}


int login(){
    cout << "1. Login\n";
    cout << "2. Regiser\n";
    cout << "3. Exit\n";
    cout << "   Enter your opiton:_ ";
    int op;
    cin >> op;
    return op;
}
// end of MENU

int main(){
    ifstream trnx_file;
    trnx_file.open("trnx.txt");
    string ct;
    trnx_file >>ct;
    trnx_count = stoi(ct);
    trnx_file.close();
    
    ifstream r_user_file;
    r_user_file.open("users.txt");
    while(r_user_file.eof() == 0){
        string phone, name, pin, balance, total_amount, is_active;
        r_user_file >> phone;
        r_user_file >> name;
        r_user_file >> pin;
        r_user_file >> balance;
        r_user_file >> is_active;
        users[++user_count].restoreMember(phone, name, pin,stoi(balance), stoi(is_active));
    }
    r_user_file.close();



    while(1){
        int op = login();
        if( op == 1){
            bool logged_in = loginMenu();
            if( logged_in ){
                mainMenu();
            }
            else{
                continue;    
            }
        }
        else if( op == 2){
            Register();
        }
        else if ( op == 3){
            break;
        }
        else{
            cout << "Enter valid option.\n";
        }
    
    }


}