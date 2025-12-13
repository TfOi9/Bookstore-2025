#include "../include/account.hpp"

Account::Account(const std::string& user_id, int privilege, const std::string& username, const std::string& password) {
    user_id_ = string_to_array<30>(user_id);
    username_ = string_to_array<30>(username);
    password_ = string_to_array<30>(password);
    privilege_ = privilege;
}

bool Account::verify_password(const std::string& password) const {
    return array_to_string<30>(password_) == password;
}

std::string Account::username() const {
    return array_to_string<30>(username_);
}

std::string Account::password() const {
    return array_to_string<30>(password_);
}

std::string Account::user_id() const {
    return array_to_string<30>(user_id_);
}

int Account::privilege() const {
    return privilege_;
}

bool Account::operator==(const Account& oth) const {
    return (user_id_ == oth.user_id_ && username_ == oth.username_ && password_ == oth.password_ && privilege_ == oth.privilege_);
}

AccountManager::AccountManager(const std::string& file_name, const std::string& root_password)
    : account_file_(file_name), account_count_(1) {
    if (account_file_.count(string_to_array<30>("root"))) {
        // std::cerr << "found user root" << std::endl;
        login_stack_.push_back(Account());
        book_stack_.push_back(-1);
        return;
    }
    Account root("root", 7, "root", root_password);
    account_file_.insert(string_to_array<30>("root"), root);
    login_stack_.push_back(Account());
    book_stack_.push_back(-1);
}

bool AccountManager::login(const std::string& user_id, const std::string& password) {
    std::vector<Account> accounts = account_file_.find(string_to_array<30>(user_id));
    if (accounts.empty()) {
        return false;
    }
    // std::cerr << accounts.size() << std::endl;
    Account& account = accounts[0];
    if (account.verify_password(password) || account.privilege_ < login_stack_.back().privilege() && password == "") {
        login_stack_.push_back(account);
        book_stack_.push_back(-1);
        return true;
    }
    return false;
}

bool AccountManager::logout() {
    if (login_stack_.back().privilege_ < 1) {
        return false;
    }
    if (login_stack_.empty()) {
        return false;
    }
    login_stack_.pop_back();
    book_stack_.pop_back();
    return true;
}

bool AccountManager::register_account(const std::string& user_id, const std::string& username, const std::string& password, int privilege) {
    std::array<char, 30> arr = string_to_array<30>(user_id);
    if (account_file_.find(arr).empty()) {
        Account new_account(user_id, privilege, username, password);
        account_file_.insert(arr, new_account);
        account_count_++;
        return true;
    }
    return false;
}

bool AccountManager::change_password(const std::string& user_id, const std::string& new_password, const std::string& old_password) {
    if (login_stack_.back().privilege_ < 1) {
        return false;
    }
    std::array<char, 30> arr = string_to_array<30>(user_id);
    std::vector<Account> accounts = account_file_.find(arr);
    if (accounts.empty()) {
        return false;
    }
    Account& account = accounts[0];
    if (account.verify_password(old_password) || login_stack_.back().privilege() == 7 && old_password == "") {
        if (new_password == old_password) {
            return true;
        }
        Account new_account = account;
        new_account.password_ = string_to_array<30>(new_password);
        account_file_.insert(arr, new_account);
        account_file_.erase(arr, account);
        return true;
    }
    return false;
}

bool AccountManager::add_account(const std::string& user_id, const std::string& username, const std::string& password, int privilege) {
    if (login_stack_.back().privilege_ < 3) {
        return false;
    }
    std::array<char, 30> arr = string_to_array<30>(user_id);
    if (login_stack_.back().privilege_ <= privilege) {
        return false;
    }
    if (account_file_.count(arr)) {
        return false;
    }
    Account account(user_id, privilege, username, password);
    account_file_.insert(arr, account);
    account_count_++;
    return true;
}

bool AccountManager::delete_account(const std::string& user_id) {
    if (login_stack_.back().privilege_ < 7) {
        return false;
    }
    std::array<char, 30> arr = string_to_array<30>(user_id);
    if (!account_file_.count(arr)) {
        return false;
    }
    for (int i = 0; i < login_stack_.size(); i++) {
        if (login_stack_[i].user_id_ == arr) {
            return false;
        }
    }
    account_file_.erase(arr);
    account_count_--;
    return true;
}

bool AccountManager::select_book(int id) {
    if (login_stack_.back().privilege_ < 3) {
        return false;
    }
    book_stack_.back() = id;
    return true;
}

std::string AccountManager::current_user() const {
    if (login_stack_.empty()) {
        return "";
    }
    return login_stack_.back().user_id();
}

std::string AccountManager::current_username() const {
    if (login_stack_.empty()) {
        return "";
    }
    return login_stack_.back().username();
}

int AccountManager::current_privilege() const {
    if (login_stack_.empty()) {
        return 0;
    }
    return login_stack_.back().privilege();
}

int AccountManager::selected_book() const {
    if (book_stack_.empty()) {
        return -1;
    }
    return book_stack_.back();
}

std::vector<std::string> AccountManager::list_admins() {
    std::vector<std::string> admins;
    auto list = account_file_.serialize();
    for (auto account : list) {
        if (account.privilege() >= 3) {
            admins.push_back(account.user_id());
        }
    }
    return admins;
}