#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <array>
#include <vector>
#include "hash_map.hpp"
#include "utils.hpp"

class Account {
    friend class AccountManager;
private:
    std::array<char, 30> user_id_;
    std::array<char, 30> username_;
    std::array<char, 30> password_;
    int privilege_;
    // privileges: 0 - tourist, 1 - customer, 3 - administrator, 7 - root.
public:
    Account(const std::string& user_id = "", int privilege = 0, const std::string& username = "", const std::string& password = "");

    ~Account() = default;

    // checks if the password is correct.
    bool verify_password(const std::string& password) const;

    // returns user ID.
    std::string user_id() const;

    // returns username.
    std::string username() const;

    // returns password.
    std::string password() const;

    // returns privilege.
    int privilege() const;

    // == operator.
    bool operator==(const Account& oth) const;
};

class AccountManager {
private:
    HashMap<std::array<char, 30>, Account> account_file_;
    int account_count_;
    std::vector<Account> login_stack_;
    std::vector<int> book_stack_;
public:
    AccountManager(const std::string& file_name = "account.dat", const std::string& root_password = "sjtu");

    ~AccountManager() = default;

    // trys to log in.
    bool login(const std::string& user_id, const std::string& password);

    // trys to log out. requires privilege 1.
    bool logout();

    // trys to register an account.
    bool register_account(const std::string& user_id, const std::string& username, const std::string& password, int privilege);

    // trys to change password. requires privilege 1.
    bool change_password(const std::string& user_id, const std::string& new_password, const std::string& old_password = "");

    // trys to add an account. requires privilege 3.
    bool add_account(const std::string& user_id, const std::string& username, const std::string& password, int privilege);

    // trys to delete an account. requires privilege 7.
    bool delete_account(const std::string& user_id);

    // selects book for the current account. requires privilege 3.
    bool select_book(int id);

    // returns the current user's id.
    std::string current_user() const;

    // returns the current user's name.
    std::string current_username() const;

    // returns the current user's privilege.
    int current_privilege() const;

    // returns the selected book's id.
    int selected_book() const;

    void debug();
};

#endif