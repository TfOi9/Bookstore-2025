#include <iostream>
#include <string>
#include "../include/memory_river.hpp"
using namespace std;

int main() {
    MemoryRiver<long long, 2> mr;
    mr.initialise("test_ll.txt");
    int t;
    cin >> t;
    while (t--) {
        string op;
        cin >> op;
        if (op == "write") {
            long long x;
            cin >> x;
            int pos = mr.write(x);
            cout << "data " << x << " stored in pos " << pos << endl;
        }
        else if (op == "update") {
            long long x;
            int pos;
            cin >> x >> pos;
            mr.update(x, pos);
            cout << "pos " << pos << " updated to " << x << endl;
        }
        else if (op == "query") {
            int pos;
            cin >> pos;
            long long x;
            mr.read(x, pos);
            cout << "value " << x << " stored in pos " << pos << endl;
        }
    }
    return 0;
}