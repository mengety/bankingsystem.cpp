#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <iomanip>
#include <string>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <thread>
#include <mutex>

class Transaction {
public:
    std::string date;
    std::string time;
    std::string type;
    double amount;
    double resultingBalance;

    Transaction(std::string t, double amt, double balance) 
        : type(t), amount(amt), resultingBalance(balance) {
        time_t now = time(0);
        date = ctime(&now);
        date.erase(date.length() - 1); // Remove newline
        time = ctime(&now);
        time.erase(time.length() - 1); // Remove newline
    }
};

class Account {
protected:
    double balance;
    std::vector<Transaction> transactions;

public:
    Account() : balance(0.0) {}

    virtual void deposit(double amount) {
        balance += amount;
        transactions.emplace_back("Deposit", amount, balance);
    }

    virtual void withdraw(double amount) {
        if (amount > balance) throw std::runtime_error("Insufficient funds");
        balance -= amount;
        transactions.emplace_back("Withdraw", amount, balance);
    }

    double getBalance() const {
        return balance;
    }

    const std::vector<Transaction>& getTransactions() const {
        return transactions;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(double rate) : interestRate(rate) {}

    void applyInterest() {
        balance += balance * interestRate;
    }
};

class User {
private:
    std::string name;
    std::string username;
    std::string password;
    std::unique_ptr<Account> account;

public:
    User(std::string n, std::string u, std::string p) 
        : name(n), username(u), password(p), account(std::make_unique<SavingsAccount>(0.05)) {}

    void deposit(double amount) {
        account->deposit(amount);
    }

    void withdraw(double amount) {
        account->withdraw(amount);
    }

    double getBalance() const {
        return account->getBalance();
    }

    const std::vector<Transaction>& getTransactions() const {
        return account->getTransactions();
    }

    std::string getUsername() const {
        return username;
    }

    bool validatePassword(const std::string& p) const {
        return password == p;
    }
};

class BankSystem {
private:
    std::map<std::string, std::shared_ptr<User>> users;
    std::mutex mtx;

public:
    void addUser(const std::string& name, const std::string& username, const std::string& password) {
        std::lock_guard<std::mutex> lock(mtx);
        if (users.find(username) != users.end()) {
            throw std::runtime_error("Username already exists");
        }
        users[username] = std::make_shared<User>(name, username, password);
    }

    std::shared_ptr<User> login(const std::string& username, const std::string& password) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = users.find(username);
        if (it != users.end() && it->second->validatePassword(password)) {
            return it->second;
        }
        throw std::runtime_error("Invalid username or password");
    }

    void saveData() {
        std::ofstream file("bank_data.txt", std::ios::binary);
        for (const auto& pair : users) {
            // Serialize user data
            // Implementation omitted for brevity
        }
    }

    void loadData() {
        std::ifstream file("bank_data.txt", std::ios::binary);
        // Deserialize user data
        // Implementation omitted for brevity
    }
};

int main() {
    BankSystem bank;
    try {
        bank.addUser("John Doe", "johndoe", "password123");
        auto user = bank.login("johndoe", "password123");
        user->deposit(1000);
        user->withdraw(200);
        std::cout << "Balance: " << user->getBalance() << std::endl;

        for (const auto& transaction : user->getTransactions()) {
            std::cout << transaction.date << " " << transaction.time << " " 
                      << transaction.type << " " << transaction.amount 
                      << " Balance: " << transaction.resultingBalance << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
