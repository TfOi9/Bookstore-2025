#include <iostream>
#include "../include/fixed_string.hpp"
#include "../include/key_map.hpp"
using namespace std;

int main() {
    int t;
    cin >> t;
    HashMap<FixedString<char, 64>, int> hshmp;
    while (t--) {
        string op;
        FixedString<char, 64> s;
        int x;
        cin >> op;
        if (op == "insert") {
            cin >> s >> x;
            hshmp.insert(s, x);
        }
        else if (op == "delete") {
            cin >> s >> x;
            hshmp.erase(s, x);
        }
        else if (op == "find") {
            cin >> s;
            std::vector<int> vec = hshmp.find(s);
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
    return 0;
}