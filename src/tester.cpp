#include <iostream>
#include <cstring>
#include <string>
#include "../include/hash_map.hpp"
using namespace std;

void read_arr(array<char, 64>& arr) {
    string s;
    cin >> s;
    for (int i = 0; i < 64; i++) {
        if (i < s.size()) {
            arr[i] = s[i];
        }
        else {
            arr[i] = 0;
        }
    }
}

int main() {
    int t;
    cin >> t;
    HashMap<array<char, 64>, int, MemoryHash<array<char, 64> >, 200, 1007> hshmp;
    while (t--) {
        string op, str;
        array<char, 64> s;
        int x;
        cin >> op;
        if (op == "insert") {
            read_arr(s);
            cin >> x;
            hshmp.insert(s, x);
        }
        else if (op == "delete") {
            read_arr(s);
            cin >> x;
            hshmp.erase(s, x);
        }
        else if (op == "find") {
            read_arr(s);
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