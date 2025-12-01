#include <iostream>
#include "../include/log.hpp"
using namespace std;

int main() {
    LogManager manager;
    int t;
    cin >> t;
    while (t--) {
        string op;
        double cost;
        cin >> op;
        if (op == "add") {
            cin >> cost;
            manager.add_finance_log(cost);
        }
        else if (op == "report") {
            int cnt;
            cin >> cnt;
            auto p = manager.finance(cnt);
            cout << "+ " << p.first << " - " << p.second << endl;
        }
    }
    return 0;
}