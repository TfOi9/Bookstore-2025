#include "../include/account.hpp"
using namespace std;

int main() {
    AccountManager manager;
    int t;
    cin >> t;
    while (t--) {
        string op;
        cin >> op;
        if (op == "su") {
            string id, pwd;
            cin >> id >> pwd;
            if (manager.login(id, pwd)) {
                cout << "login successful." << endl;
            }
            else {
                cout << "login failed" << endl;
            }
        }
        else if (op == "logout") {
            if (manager.logout()) {
                cout << "logout successful." << endl;
            }
            else {
                cout << "logout failed" << endl;
            }
        }
        else if (op == "register") {
            string id, pwd, name;
            int pr = 1;
            cin >> id >> pwd >> pr >> name;
            if (manager.register_account(id, name, pwd, pr)) {
                cout << "register successful." << endl;
            }
            else {
                cout << "register failed" << endl;
            }
        }
        else if (op == "passwd") {
            string id, pwd, npwd;
            cin >> id >> pwd >> npwd;
            if (manager.change_password(id, npwd, pwd)) {
                cout << "change successful." << endl;
            }
            else {
                cout << "change failed" << endl;
            }
        }
        else if (op == "useradd") {
            string id, pwd, name;
            int pr = 1;
            cin >> id >> pwd >> pr >> name;
            if (manager.add_account(id, name, pwd, pr)) {
                cout << "add successful" << endl;
            }
            else {
                cout << "add failed" << endl;
            }
        }
        else if (op == "delete") {
            string id;
            cin >> id;
            if (manager.delete_account(id)) {
                cout << "delete successful" << endl;
            }
            else {
                cout << "delete failed" << endl;
            }
        }
    }
}