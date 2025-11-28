#include "../include/utils.hpp"

template<int StringSize>
std::array<char, StringSize> string_to_array(const std::string& str) {
    std::array<char, StringSize> arr;
    for (int i = 0; i < StringSize; i++) {
        if (i < str.size()) {
            arr[i] = str[i];
        }
        else {
            arr[i] = 0;
        }
    }
    return arr;
}

template<int StringSize>
std::string array_to_string(const std::array<char, StringSize>& arr) {
    std::string str;
    for (int i = 0; i < StringSize; i++) {
        if (arr[i] != 0) {
            str += arr[i];
        }
        else {
            break;
        }
    }
    return str;
}