#include <iostream>
#include <string>
#include "../include/validator.hpp"
using namespace std;

int main() {
    string str;
    int t;
    cin >> t;
    cout << Validator("a. a").visible_only() << endl;
    while (t--) {
        cin >> str;
        cout << Validator(str).max_len(20).normal_char_only().no_commas() << endl;
        cout << Validator(str).max_len(20).number_and_dot_only().only_one_dot() << endl;
        cout << Validator(str).max_len(20).number_only() << endl;
    }
    return 0;
}