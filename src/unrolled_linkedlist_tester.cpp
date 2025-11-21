#include <iostream>
#include <string>
#include <cstring>
#include "../include/unrolled_linkedlist.hpp"
struct StringKey {
    char ch[68];
    StringKey() {
        memset(ch, 0, sizeof(ch));
    }
};
bool operator<(const StringKey& a, const StringKey& b) {
    return strcmp(a.ch, b.ch) < 0;
}
std::ostream& operator<<(std::ostream& os, StringKey str) {
    os << str.ch;
    return os;
}
std::istream& operator>>(std::istream& is, StringKey& str) {
    is >> str.ch;
    return is;
}
int main() {
    UnrolledLinkedList<StringKey, int, 200> mp;
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        string op;
        std::cin >> op;
        if (op == "insert") {
            StringKey key;
            int val;
            std::cin >> key >> val;
            mp.insert(key, val);
        }
        else if (op == "delete") {
            StringKey key;
            int val;
            std::cin >> key >> val;
            mp.erase(key, val);
        }
        else if (op == "find") {
            StringKey key;
            std::cin >> key;
            mp.find(key);
        }
        // mp.print(cerr);
    }
    return 0;
}