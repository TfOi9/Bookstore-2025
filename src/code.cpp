#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <map>
#include <iomanip>
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
                // std::clog << "Exiting system.\n";
                std::string msg = current_time() + " [EXIT]User " + account_manager.current_user() + " exited system.";
                // std::clog << msg << '\n';
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
                // std::clog << "Login success.\n";
                std::string msg = current_time() + " [LOGIN]User " + user_id + " logged in.";
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Login failed.\n";
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
                // std::clog << "Logout success.\n";
                std::string msg = current_time() + " [LOGOUT]User " + user_id + " logged out.";
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Logout failed.\n";
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
                // std::clog << "Register success.\n";
                std::string msg = current_time() + " [REGISTER]User " + user_id + " registered.";
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Register failed." << std::endl;
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
                // std::clog << "Change password success.\n";
                std::string msg = current_time() + " [PASSWORD]User " + user_id + " changed password.";
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Change password failed." << std::endl;
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
                // std::clog << "Add user success.\n";
                std::string msg = current_time() + " [USERADD]Added user " + user_id + '.';
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Add user failed.\n";
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
                // std::clog << "Delete user success.\n";
                std::string msg = current_time() + " [DELETE]Deleted user " + user_id + '.';
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Delete user failed.\n";
            }
        }
        else if (op == "show") {
            if (tokens.size() > 1 && tokens[1] == "finance") {
                if (account_manager.current_previlege() < 7) {
                    std::cout << "Invalid\n";
                    continue;
                }
                if (tokens.size() < 2 || tokens.size() > 3) {
                    std::cout << "Invalid\n";
                    continue;
                }
                if (tokens[1] != "finance") {
                    std::cout << "Invalid\n";
                    continue;
                }
                int count = -1;
                if (tokens.size() == 3) {
                    // std::cerr << tokens[2] << std::endl;
                    std::string c = tokens[2];
                    if (!Validator(c).max_len(10).number_only()) {
                        std::cout << "Invalid\n";
                        continue;
                    }
                    count = std::stoi(c);
                }
                if (count == 0) {
                    std::cout << '\n';
                    continue;
                }
                auto p = log_manager.finance(count);
                std::cout << "+ " << std::fixed << std::setprecision(2) << p.first << " - " << std::fixed << std::setprecision(2) << p.second << '\n';
                continue;
            }
            if (account_manager.current_previlege() < 1) {
                std::cout << "Invalid\n";
                continue;
            }
            if (tokens.size() < 1 || tokens.size() > 2) {
                std::cout << "Invalid\n";
                continue;
            }
            if (tokens.size() == 1) {
                auto vec = book_manager.serialize();
                for (auto book : vec) {
                    std::cout << book.ISBN() << '\t' << book.book_name() << '\t' << book.author() << '\t' << book.keyword() << '\t' << std::fixed << std::setprecision(2) << book.price_ << '\t' << book.quant_ << '\n';
                }
                std::string msg = current_time() + " [FIND]User " + account_manager.current_user() + " found all books.";
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
                continue;
            }
            std::string key, val;
            bool valid = parse_argument(tokens[1], key, val);
            if (!valid) {
                bool number_valid = parse_number_argument(tokens[1], key, val);
                if (number_valid == 0 || key != "ISBN") {
                    std::cout << "Invalid\n";
                    continue;
                }
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
                vec = book_manager.find_ISBN(string_to_array<20>(val));
            }
            else if (key == "name") {
                vec = book_manager.find_book_name(string_to_array<60>(val));
            }
            else if (key == "author") {
                vec = book_manager.find_author(string_to_array<60>(val));
            }
            else if (key == "keyword") {
                vec = book_manager.find_keyword(string_to_array<60>(val));
            }
            if (vec.empty()) {
                std::cout << '\n';
            }
            else for (auto book : vec) {
                std::cout << book.ISBN() << '\t' << book.book_name() << '\t' << book.author() << '\t' << book.keyword() << '\t' << std::fixed << std::setprecision(2) << book.price_ << '\t' << book.quant_ << '\n';
            }
            std::string msg = current_time() + " [FIND]User " + account_manager.current_user() + " found books by " + key + '.';
            // std::clog << msg << '\n';
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
            bool buy_success = book_manager.buy(string_to_array<20>(ISBN), quant, cost);
            if (buy_success) {
                std::cout << std::fixed << std::setprecision(2) << cost << std::endl;
                // std::clog << "Buy book success, costing " << cost << ".\n";
                std::string msg = current_time() + " [BUY]User " + account_manager.current_user() + " bought " + q + " book(s). ISBN:" + ISBN;
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
                log_manager.add_finance_log(cost);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Buy book failed.\n";
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
            auto vec = book_manager.find_ISBN(string_to_array<20>(ISBN));
            if (vec.empty()) {
                Book new_book(ISBN);
                new_book.id_ = book_manager.size();
                // std::cerr << "new id is " << new_book.id_ << std::endl;
                book_manager.add(new_book);
                account_manager.select_book(new_book.id_);
            }
            else {
                account_manager.select_book(vec[0].id_);
            }
            // std::clog << "Select book success.\n";
            std::string msg = current_time() + " [SELECT]User " + account_manager.current_user() + " selected book " + ISBN + '.';
            // std::clog << msg << '\n';
            log_manager.add_log(msg);
        }
        else if (op == "modify") {
            if (account_manager.current_previlege() < 3) {
                std::cout << "Invalid\n";
                continue;
            }
            if (tokens.size() < 2) {
                std::cout << "Invalid\n";
                continue;
            }
            if (account_manager.selected_book() == -1) {
                std::cout << "Invalid\n";
                continue;
            }
            std::map<std::string, std::string> mp;
            bool parse_success = true;
            for (int i = 1; i < tokens.size(); i++) {
                std::string key;
                std::string val;
                bool valid = parse_argument(tokens[i], key, val);
                if (!valid) {
                    bool number_valid = parse_number_argument(tokens[i], key, val);
                    if (number_valid == 0 || key != "price" && key != "ISBN") {
                        parse_success = false;
                        break;
                    }
                }
                if (mp.count(key)) {
                    parse_success = false;
                    break;
                }
                if (key == "ISBN") {
                    if (!Validator(val).max_len(20).visible_only()) {
                        parse_success = false;
                        break;
                    }
                }
                else if (key == "name" || key == "author" || key == "keyword") {
                    if (!Validator(val).max_len(60).visible_only().no_commas()) {
                        parse_success = false;
                        break;
                    }
                    if (key == "keyword") {
                        auto key_words = parse_keywords(string_to_array<60>(val));
                        std::sort(key_words.begin(), key_words.end());
                        if (std::unique(key_words.begin(), key_words.end()) != key_words.end()) {
                            parse_success = false;
                            break;
                        }
                    }
                }
                else if (key == "price") {
                    if (!Validator(val).max_len(13).number_and_dot_only().only_one_dot()) {
                        parse_success = false;
                        break;
                    }
                }
                else {
                    parse_success = false;
                    break;
                }
                mp[key] = val;
            }
            if (!parse_success) {
                std::cout << "Invalid\n";
                continue;
            }
            bool modify_success = true;
            Book book = book_manager.find(account_manager.selected_book());
            for (auto it = mp.begin(); it != mp.end(); it++) {
                // std::cerr << it->first << " " << it->second << std::endl;
                // std::cerr << modify_success << std::endl;
                if (it->first == "ISBN") {
                    if (book.ISBN() == it->second || book_manager.count_ISBN(string_to_array<20>(it->second))) {
                        modify_success = false;
                        break;
                    }
                    book_manager.modify_ISBN(book.ISBN_, string_to_array<20>(it->second));
                    book.ISBN_ = string_to_array<20>(it->second);
                }
                else if (it->first == "name") {
                    book_manager.modify_book_name(book.ISBN_, string_to_array<60>(it->second));
                }
                else if (it->first == "author") {
                    book_manager.modify_author(book.ISBN_, string_to_array<60>(it->second));
                }
                else if (it->first == "keyword") {
                    book_manager.modify_keyword(book.ISBN_, string_to_array<60>(it->second));
                }
                else if (it->first == "price") {
                    book_manager.modify_price(book.ISBN_, std::stod(it->second));
                }
                else {
                    modify_success = false;
                }
            }
            if (modify_success) {
                // std::clog << "Modify book success.\n";
                std::string msg = current_time() + " [MODIFY]User " + account_manager.current_user() + " modified book " + book.ISBN();
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
                log_manager.add_employee_log(account_manager.current_user(), msg);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Modify book failed.\n";
            }
        }
        else if (op == "import") {
            if (account_manager.current_previlege() < 3) {
                std::cout << "Invalid\n";
                continue;
            }
            if (tokens.size() != 3) {
                std::cout << "Invalid\n";
                continue;
            }
            if (account_manager.selected_book() == -1) {
                std::cout << "Invalid\n";
                continue;
            }
            std::string q = tokens[1], tc = tokens[2];
            bool quant_valid = Validator(q).max_len(10).number_only();
            bool cost_valid = Validator(q).max_len(13).number_and_dot_only().only_one_dot();
            if (quant_valid == 0 || cost_valid == 0) {
                std::cout << "Invalid\n";
                continue;
            }
            int quant = std::stoi(q);
            double cost = std::stod(tc);
            Book book = book_manager.find(account_manager.selected_book());
            bool import_success = book_manager.import(book.ISBN_, quant);
            if (import_success) {
                // std::clog << "Import book success.\n";
                std::string msg = current_time() + " [IMPORT]User " + account_manager.current_user() + " imported book " + book.ISBN() + ' ' + q + " copies, costing " + tc + '.';
                // std::clog << msg << '\n';
                log_manager.add_log(msg);
                log_manager.add_employee_log(account_manager.current_user(), msg);
                log_manager.add_finance_log(-cost);
            }
            else {
                std::cout << "Invalid\n";
                // std::clog << "Import book failed.\n";
            }
        }
        else if (op == "report") {

        }
        else if (op == "log") {

        }
        else {
            std::cout << "Invalid\n";
            continue;
        }
    }
    return 0;
}