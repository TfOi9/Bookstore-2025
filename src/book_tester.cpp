#include "../include/book.hpp"
using namespace std;

int main() {
    BookManager manager;
    int t;
    cin >> t;
    while (t--) {
        string op;
        cin >> op;
        if (op == "select") {
            string ISBN;
            cin >> ISBN;
            manager.select(ISBN);
        }
        else if (op == "info") {
            Book book = manager.selected_book();
            cout << book.ISBN() << " " << book.book_name() << " " << book.author() << " " << book.price() << " " << book.quant() << endl;
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
            if (manager.import(ISBN, quant)) {
                cout << "import success" << endl;
            }
            else {
                cout << "import failed" << endl;
            }
        }
        else if (op == "modify") {
            string ISBN, name, auth, kw;
            double pr;
            cin >> ISBN >> name >> auth >> kw >> pr;
            if (manager.modify(ISBN, name, auth, kw, pr)) {
                cout << "modify success" << endl;
            }
            else {
                cout << "modify failed" << endl;
            }
        }
    }
    return 0;
}