#include <iostream>
#include <cstring>
#include <string>
#include "../include/string_map.hpp"
using namespace std;

int main() {
    int t;
    cin >> t;
    HashMap<int> hshmp;
    while (t--) {
        string op, s;
        int x;
        cin >> op;
        if (op == "insert") {
            cin >> s >> x;
            hshmp.insert(s.c_str(), s.size(), x);
        }
        else if (op == "delete") {
            cin >> s >> x;
            hshmp.erase(s.c_str(), s.size(), x);
        }
        else if (op == "find") {
            cin >> s;
            std::vector<int> vec = hshmp.find(s.c_str(), s.size());
            if (!vec.size()) {
                cout << "null" << endl;
                continue;
            }
            for (int i = 0; i < vec.size(); i++) {
                cout << vec[i] << " ";
            }
            cout << endl;
        }
    }
}