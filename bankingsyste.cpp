// Smart Community Banking System (SCBS)
// Features: Account Creation, Login, Balance Management, P2P Transfers, Savings Goals, Gamified Finance, etc.
// This version uses only C++ standard concepts (no DB or external libs)

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

// Account Type Enum
enum AccountType { SAVINGS, CURRENT, BUSINESS };

// User Account Structure
struct Account {
    string username;
    string password;
    double balance;
    AccountType type;
    vector<string> transactionHistory;
    double goalAmount;
    bool goalSet = false;
};

// Main Bank System Class
class BankingSystem {
private:
    unordered_map<string, Account> users;
    Account* loggedInUser = nullptr;

public:
    // Create Account
    void createAccount() {
        Account acc;
        cout << "Enter username: "; cin >> acc.username;
        cout << "Enter password: "; cin >> acc.password;
        acc.balance = 0.0;

        int typeChoice;
        cout << "Account Type (0 = SAVINGS, 1 = CURRENT, 2 = BUSINESS): ";
        cin >> typeChoice;
        acc.type = static_cast<AccountType>(typeChoice);

        users[acc.username] = acc;
        cout << "Account created successfully!\n";
    }

    // Login
    bool login() {
        string uname, pass;
        cout << "Enter username: "; cin >> uname;
        cout << "Enter password: "; cin >> pass;

        if (users.count(uname) && users[uname].password == pass) {
            loggedInUser = &users[uname];
            cout << "Login successful!\n";
            return true;
        }
        cout << "Invalid credentials.\n";
        return false;
    }

    // View Balance
    void viewBalance() {
        if (loggedInUser)
            cout << "Current Balance: $" << loggedInUser->balance << "\n";
    }

    // Deposit
    void deposit() {
        double amt;
        cout << "Enter amount to deposit: $";
        cin >> amt;
        loggedInUser->balance += amt;
        loggedInUser->transactionHistory.push_back("Deposited $" + to_string(amt));
        cout << "Deposit successful!\n";
    }

    // Withdraw
    void withdraw() {
        double amt;
        cout << "Enter amount to withdraw: $";
        cin >> amt;
        if (amt <= loggedInUser->balance) {
            loggedInUser->balance -= amt;
            loggedInUser->transactionHistory.push_back("Withdrew $" + to_string(amt));
            cout << "Withdrawal successful!\n";
        }
        else {
            cout << "Insufficient funds.\n";
        }
    }

    // Transfer
    void transfer() {
        string recipient;
        double amt;
        cout << "Enter recipient username: ";
        cin >> recipient;
        cout << "Enter amount to transfer: $";
        cin >> amt;

        if (users.count(recipient) && amt <= loggedInUser->balance) {
            loggedInUser->balance -= amt;
            users[recipient].balance += amt;
            loggedInUser->transactionHistory.push_back("Transferred $" + to_string(amt) + " to " + recipient);
            users[recipient].transactionHistory.push_back("Received $" + to_string(amt) + " from " + loggedInUser->username);
            cout << "Transfer successful!\n";
        }
        else {
            cout << "Transfer failed. Check balance or recipient.\n";
        }
    }

    // Set Savings Goal
    void setGoal() {
        cout << "Enter your savings goal amount: $";
        cin >> loggedInUser->goalAmount;
        loggedInUser->goalSet = true;
        cout << "Goal set to $" << loggedInUser->goalAmount << "\n";
    }

    // Track Goal
    void trackGoal() {
        if (loggedInUser->goalSet) {
            double progress = (loggedInUser->balance / loggedInUser->goalAmount) * 100;
            cout << "You are " << progress << "% toward your savings goal.\n";
        }
        else {
            cout << "No goal set.\n";
        }
    }

    // Gamified Finance Tip
    void financeGameTip() {
        cout << "🎯 Tip: Save at least 10% of each deposit to reach your goal faster!\n";
    }

    // View Transactions
    void viewTransactions() {
        cout << "Transaction History:\n";
        for (auto& t : loggedInUser->transactionHistory)
            cout << "- " << t << "\n";
    }

    // Main Menu
    void menu() {
        int choice;
        do {
            cout << "\n1. View Balance\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Set Goal\n6. Track Goal\n7. View Tips\n8. Transactions\n0. Logout\nChoice: ";
            cin >> choice;
            switch (choice) {
            case 1: viewBalance(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: transfer(); break;
            case 5: setGoal(); break;
            case 6: trackGoal(); break;
            case 7: financeGameTip(); break;
            case 8: viewTransactions(); break;
            case 0: loggedInUser = nullptr; cout << "Logged out.\n"; break;
            default: cout << "Invalid option.\n";
            }
        } while (loggedInUser);
    }

    // Entry Point
    void run() {
        int ch;
        do {
            cout << "\n--- Smart Community Banking ---\n1. Create Account\n2. Login\n0. Exit\nChoice: ";
            cin >> ch;
            switch (ch) {
            case 1: createAccount(); break;
            case 2: if (login()) menu(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid.\n";
            }
        } while (ch != 0);
    }
};

int main() {
    BankingSystem bank;
    bank.run();
    return 0;
}
