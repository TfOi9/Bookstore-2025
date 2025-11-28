#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <array>
#include <vector>
#include "hash_map.hpp"

class Account {
private:
    int user_id_;
    int previlege_;
    std::string username_;
    std::string password_;
    enum class Previlege {
        kTourist = 0,
        kCustomer = 1,
        kAdmin = 3,
        kRoot = 7
    };
public:
    Account() = delete;
    Account(int user_id, int previlege, const std::string& username, const std::string& password);
    ~Account() = default;
    bool verify_password(const std::string& password) const;
    int user_id() const;
    int previlege() const;
    std::string username() const;
    std::string password() const;
};

class AccountManager {
private:
    HashMap<std::array<char, 30>, Account> account_file_;
    int account_count_;
    Account current_account_;
public:
    AccountManager() = delete;
    AccountManager(const std::string& file_name = "account.dat");
    ~AccountManager() = default;
    bool login(const std::string& user_id, const std::string& password);
    bool logout();
    bool register_account(const std::string& user_id, const std::string& username, const std::string& password, int previlege);
    bool change_password(const std::string& new_password, const std::string& old_password = "");
    bool add_account(const std::string& user_id, const std::string& username, const std::string& password, int previlege);
    bool delete_account(const std::string& user_id);
};

#endif