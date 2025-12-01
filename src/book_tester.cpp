#include "../include/book.hpp"
using namespace std;

int main() {
    BookManager manager;
    int t;
    cin >> t;
    while (t--) {
        string op;
        cin >> op;
        if (op == "add") {
            string ISBN;
            cin >> ISBN;
            if (manager.add(ISBN)) {
                cout << "add success" << endl;
            }
            else {
                cout << "add failed" << endl;
            }
        }
        else if (op == "buy") {
            double cost = 0.00;
            string ISBN;
            int quant = 0;
            cin >> ISBN >> quant;
            if (manager.buy(ISBN, quant, cost)) {
                cout << "buy success, costing " << cost << endl;
            }
            else {
                cout << "buy failed" << endl;
            }
        }
        else if (op == "import") {
            string ISBN;
            int quant;
            cin >> ISBN >> quant;
            if (manager.import_by_quantity(ISBN, quant)) {
                cout << "import success" << endl;
            }
            else {
                cout << "import failed" << endl;
            }
        }
        else if (op == "mISBN") {
            string ISBN, new_ISBN;
            cin >> ISBN >> new_ISBN;
            if (manager.modify_ISBN(ISBN, new_ISBN)) {
                cout << "modify success" << endl;
            }
            else {
                cout << "modify failed" << endl;
            }
        }
        else if (op == "mname") {
            string ISBN, new_name;
            cin >> ISBN >> new_name;
            if (manager.modify_book_name(ISBN, new_name)) {
                cout << "modify success" << endl;
            }
            else {
                cout << "modify failed" << endl;
            }
        }
        else if (op == "mauth") {
            string ISBN, new_auth;
            cin >> ISBN >> new_auth;
            if (manager.modify_author(ISBN, new_auth)) {
                cout << "modify success" << endl;
            }
            else {
                cout << "modify failed" << endl;
            }
        }
        else if (op == "mkw") {
            string ISBN, new_kw;
            cin >> ISBN >> new_kw;
            if (manager.modify_keyword(ISBN, new_kw)) {
                cout << "modify success" << endl;
            }
            else {
                cout << "modify failed" << endl;
            }
        }
        else if (op == "mpr") {
            string ISBN;
            double pr;
            cin >> ISBN >> pr;
            if (manager.modify_price(ISBN, pr)) {
                cout << "modify success" << endl;
            }
            else {
                cout << "modify failed" << endl;
            }
        }
        else if (op == "fISBN") {
            string ISBN;
            cin >> ISBN;
            auto vec = manager.find_ISBN(ISBN);
            for (auto book : vec) {
                cout << book << endl;
            }
        }
        else if (op == "fname") {
            string name;
            cin >> name;
            auto vec = manager.find_book_name(name);
            for (auto book : vec) {
                cout << book << endl;
            }
        }
        else if (op == "fauth") {
            string auth;
            cin >> auth;
            auto vec = manager.find_author(auth);
            for (auto book : vec) {
                cout << book << endl;
            }
        }
        else if (op == "fkw") {
            string kw;
            cin >> kw;
            auto vec = manager.find_keyword(kw);
            for (auto book : vec) {
                cout << book << endl;
            }
        }
        else if (op == "fall") {
            auto vec = manager.find_all();
            for (auto book : vec) {
                cout << book << endl;
            }
        }
    }
    return 0;
}