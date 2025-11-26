#include <iostream>
using namespace std;

string str;
int main () {
    str = "1234567890123456789012345678901234567890123456789012345678901234";
    cerr << str.size() << endl;
    cout << 100 << endl;
    for (int i = 0; i < 100 - 1; i++) {
        cout << "insert " << str << " " << i + 1 << endl;
    }
    cout << "find " << str << endl;
}