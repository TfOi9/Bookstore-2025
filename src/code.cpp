#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include "../include/account.hpp"
#include "../include/book.hpp"
#include "../include/log.hpp"
#include "../include/validator.hpp"
#include "../include/utils.hpp"

int main() {
    AccountManager account_manager;
    BookManager book_manager;
    LogManager log_manager;
    while (1) {
        std::string s, t;
        std::vector<std::string> tokens;
        getline(std::cin, s);
        std::stringstream ss(s);
        while (ss >> t) {
            tokens.push_back(t);
        }
        if (tokens.empty()) {
            continue;
        }
        std::string op = tokens[0];
        if (op == "quit" || op == "exit") {
            if (tokens.size() > 1) {
                std::cout << "Invalid\n";
            }
            else {
                std::cerr << "Exiting system.\n";
                std::string msg = current_time() + " [EXIT]User " + account_manager.current_user() + "exited system.";
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
                break;
            }
        }
        else if (op == "su") {
            if (tokens.size() < 2 || tokens.size() > 3) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string user_id, pwd = "";
            user_id = tokens[1];
            if (tokens.size() == 3) {
                pwd = tokens[2];
            }
            bool user_id_valid = Validator(user_id).max_len(30).normal_char_only();
            bool pwd_valid = Validator(pwd).max_len(30).normal_char_only();
            if (user_id_valid == 0 || pwd_valid == 0) {
                std::cout << "Invalid\n";
                continue;
            }
            bool login_success = account_manager.login(user_id, pwd);
            if (login_success) {
                std::cerr << "Login success.\n";
                std::string msg = current_time() + " [LOGIN]User " + user_id + " logged in.";
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cerr << "Login failed.\n";
            }
        }
        else if (op == "logout") {
            if (tokens.size() != 1) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string user_id = account_manager.current_user();
            bool logout_success = account_manager.logout();
            if (logout_success) {
                std::cerr << "Logout success.\n";
                std::string msg = current_time() + " [LOGOUT]User " + user_id + " logged out.";
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cerr << "Logout failed.\n";
            }
        }
        else if (op == "register") {
            if (tokens.size() != 4) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string user_id = tokens[1], pwd = tokens[2], username = tokens[3];
            bool user_id_valid = Validator(user_id).max_len(30).normal_char_only();
            bool pwd_valid = Validator(pwd).max_len(30).normal_char_only();
            bool username_valid = Validator(username).max_len(30).visible_only();
            if (user_id_valid == 0 || pwd_valid == 0 || username_valid == 0) {
                std::cout << "Invalid\n";
                continue;
            }
            bool register_success = account_manager.register_account(user_id, username, pwd, 1);
            if (register_success) {
                std::cerr << "Register success.\n";
                std::string msg = current_time() + " [REGISTER]User " + user_id + " registered.";
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cerr << "Register failed." << std::endl;
            }
        }
        else if (op == "passwd") {
            if (tokens.size() < 3 || tokens.size() > 4) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string user_id = tokens[1], pwd = (tokens.size() == 4 ? tokens[2] : ""), new_pwd = tokens.back();
            bool new_pwd_valid = Validator(new_pwd).max_len(30).normal_char_only();
            if (!new_pwd_valid) {
                std::cout << "Invalid\n";
                continue;
            }
            bool change_success = account_manager.change_password(user_id, new_pwd, pwd);
            if (change_success) {
                std::cerr << "Change password success.\n";
                std::string msg = current_time() + " [PASSWORD]User " + user_id + " changed password.";
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cerr << "Change password failed." << std::endl;
            }
        }
        else if (op == "useradd") {
            if (tokens.size() != 5) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string user_id = tokens[1], pwd = tokens[2], username = tokens[4];
            int previlege;
            bool user_id_valid = Validator(user_id).max_len(30).normal_char_only();
            bool pwd_valid = Validator(pwd).max_len(30).normal_char_only();
            bool username_valid = Validator(username).max_len(30).visible_only();
            bool previlege_valid = Validator(tokens[3]).max_len(1).min_len(1).number_only();
            if (user_id_valid == 0 || pwd_valid == 0 || username_valid == 0 || previlege_valid == 0) {
                std::cout << "Invalid\n";
                continue;
            }
            previlege = tokens[3][0] - '0';
            if (previlege != 0 && previlege != 1 && previlege != 3 && previlege != 7) {
                std::cout << "Invalid\n";
                continue;
            }
            bool useradd_success = account_manager.add_account(user_id, username, pwd, previlege);
            if (useradd_success) {
                std::cerr << "Add user success.\n";
                std::string msg = current_time() + " [USERADD]Added user " + user_id + '.';
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cerr << "Add user failed.\n";
            }
        }
        else if (op == "delete") {
            if (tokens.size() != 2) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string user_id = tokens[1];
            bool delete_success = account_manager.delete_account(user_id);
            if (delete_success) {
                std::cerr << "Delete user success.\n";
                std::string msg = current_time() + " [DELETE]Deleted user " + user_id + '.';
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cerr << "Delete user failed.\n";
            }
        }
        else if (op == "show") {
            if (account_manager.current_previlege() < 1) {
                std::cout << "Invalid\n";
                continue;
            }
            if (tokens.size() < 1 || tokens.size() > 2) {
                std::cout << "Invalid\n";
                continue;
            }
            if (tokens.size() == 1) {
                auto vec = book_manager.find_all();
                for (auto book : vec) {
                    std::cout << book.ISBN() << '\t' << book.book_name() << '\t' << book.author() << '\t' << book.keyword() << '\t' << book.price() << '\t' << book.quant() << '\n';
                }
                std::string msg = current_time() + " [FIND]User " + account_manager.current_user() + " found all books.";
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
                continue;
            }
            std::string key, val;
            bool valid = parse_argument(tokens[1], key, val);
            if (!valid) {
                std::cout << "Invalid\n";
                continue;
            }
            if (key != "ISBN" && key != "name" && key != "author" && key != "keyword") {
                std::cout << "Invalid\n";
                continue;
            }
            bool key_valid, val_valid;
            if (key == "ISBN") {
                key_valid = Validator(key).max_len(20).normal_char_only();
            }
            else {
                key_valid = Validator(key).max_len(60).normal_char_only().no_commas();
            }
            std::vector<Book> vec;
            if (key == "ISBN") {
                vec = book_manager.find_ISBN(key);
            }
            else if (key == "name") {
                vec = book_manager.find_book_name(key);
            }
            else if (key == "author") {
                vec = book_manager.find_author(key);
            }
            else if (key == "keyword") {
                vec = book_manager.find_keyword(key);
            }
            for (auto book : vec) {
                std::cout << book.ISBN() << '\t' << book.book_name() << '\t' << book.author() << '\t' << book.keyword() << '\t' << book.price() << '\t' << book.quant() << '\n';
            }
            std::string msg = current_time() + " [FIND]User " + account_manager.current_user() + " found books by " + key + '.';
            std::cerr << msg << '\n';
            log_manager.add_log(msg);
        }
        else if (op == "buy") {
            if (account_manager.current_previlege() < 1) {
                std::cout << "Invalid\n";
                continue;
            }
            if (tokens.size() != 3) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string ISBN = tokens[1], q = tokens[2];
            bool ISBN_valid = Validator(ISBN).max_len(20).visible_only();
            bool q_valid = Validator(q).max_len(10).number_only();
            if (ISBN_valid == 0 || q_valid == 0) {
                std::cout << "Invalid\n";
                continue;
            }
            int quant = std::stoi(q);
            double cost = 0.00;
            bool buy_success = book_manager.buy(ISBN, buy_success, cost);
            if (buy_success) {
                std::cerr << "Buy book success, costing " << cost << ".\n";
                std::string msg = current_time() + " [BUY]User " + account_manager.current_user() + " bought " + q + " book(s). ISBN:" + ISBN;
                std::cerr << msg << '\n';
                log_manager.add_log(msg);
                log_manager.add_finance_log(cost);
            }
            else {
                std::cerr << "Buy book failed.\n";
            }
        }
        else if (op == "select") {
            if (account_manager.current_previlege() < 3) {
                std::cout << "Invalid\n";
                continue;
            }
            if (tokens.size() != 2) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string ISBN = tokens[1];
            bool ISBN_valid = Validator(ISBN).max_len(20).visible_only();
            if (!ISBN_valid) {
                std::cout << "Invalid\n";
                continue;
            }
            book_manager.add(ISBN);
            account_manager.select_book(ISBN);
            std::cerr << "Select book success.\n";
            std::string msg = current_time() + " [SELECT]User " + account_manager.current_user() + " selected book " + ISBN + '.';
            std::cerr << msg << '\n';
            log_manager.add_log(msg);
        }
        else if (op == "modify") {
            
        }
    }
    return 0;
}