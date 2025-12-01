#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include "../include/account.hpp"
#include "../include/book.hpp"
#include "../include/log.hpp"
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

        }
    }
    return 0;
}