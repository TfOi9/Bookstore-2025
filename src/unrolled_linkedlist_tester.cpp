#include <iostream>
#include <string>
#include <cstring>
#include "../include/unrolled_linkedlist.hpp"
using namespace std;

struct StringKey {
    char ch[68];
    StringKey() {
        memset(ch, 0, sizeof(ch));
    }
};
bool operator<(const StringKey& a, const StringKey& b) {
    return strcmp(a.ch, b.ch) < 0;
}
ostream& operator<<(ostream& os, StringKey str) {
    os << str.ch;
    return os;
}
istream& operator>>(istream& is, StringKey& str) {
    is >> str.ch;
    return is;
}
int main() {
    UnrolledLinkedList<StringKey, int, 4> mp;
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        string op;
        cin >> op;
        if (op == "insert") {
            StringKey key;
            int val;
            cin >> key >> val;
            cerr << key << endl;
            mp.insert(key, val);
        }
        else if (op == "delete") {
            StringKey key;
            int val;
            cin >> key >> val;
            mp.erase(key, val);
        }
        else if (op == "find") {
            StringKey key;
            cin >> key;
            mp.find(key);
        }
        // mp.print(cerr);
    }
    return 0;
}