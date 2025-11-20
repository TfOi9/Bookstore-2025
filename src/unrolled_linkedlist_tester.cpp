#include <iostream>
#include <string>
#include "../include/unrolled_linkedlist.hpp"
using namespace std;

int main() {
    UnrolledLinkedList<long long, int, 4> mp;
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        string op;
        cin >> op;
        if (op == "insert") {
            long long key;
            int val;
            cin >> key >> val;
            mp.insert(key, val);
        }
        else if (op == "erase") {
            long long key;
            cin >> key;
            mp.erase(key);
        }
        else if (op == "find") {
            long long key;
            cin >> key;
            mp.find(key);
        }
        mp.print(cerr);
    }
    return 0;
}