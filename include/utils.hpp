#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <array>
#include <chrono>
#include <ctime>
#include <sstream>

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

std::string current_time();

bool parse_argument(const std::string& arg, std::string& arg1, std::string& arg2);

#endif