#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <array>
#include <vector>
#include "hash_map.hpp"
#include "utils.hpp"

class Account {
private:
    std::array<char, 30> user_id_;
    std::array<char, 30> username_;
    std::array<char, 30> password_;
    int previlege_;
public:
    Account() = delete;
    Account(const std::string& user_id, int previlege, const std::string& username, const std::string& password);
    ~Account() = default;
    bool verify_password(const std::string& password) const;
    std::string user_id() const;
    std::string username() const;
    std::string password() const;
    int previlege() const;
};

class AccountManager {
private:
    HashMap<std::array<char, 30>, Account> account_file_;
    int account_count_;
    std::vector<Account> login_stack_;
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