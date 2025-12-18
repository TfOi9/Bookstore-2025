#include "../include/utils.hpp"
#include <sstream>

std::string current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::ostringstream oss;
    oss << local_time->tm_year + 1900 << '-' << local_time->tm_mon + 1 << '-' << local_time->tm_mday << ' ' << local_time->tm_hour << ':' << local_time->tm_min << ':' << local_time->tm_sec;
    return oss.str();
}

bool parse_argument(const std::string& arg, std::string& arg1, std::string& arg2) {
    if (arg[0] != '-') {
        return false;
    }
    int len = arg.size(), pos = -1;
    for (int i = 1; i < len; i++) {
        if (arg[i] == '=') {
            pos = i;
            break;
        }
    }
    if (pos >= len - 3 || pos == -1) {
        return false;
    }
    if (arg[pos + 1] != '"' || arg[len - 1] != '"') {
        return false;
    }
    arg1 = arg.substr(1, pos - 1);
    arg2 = arg.substr(pos + 2, len - pos - 3);
    return true;
}

bool parse_number_argument(const std::string& arg, std::string& arg1, std::string& arg2) {
    if (arg[0] != '-') {
        return false;
    }
    int len = arg.size(), pos = -1;
    for (int i = 1; i < len; i++) {
        if (arg[i] == '=') {
            pos = i;
            break;
        }
    }
    if (pos >= len - 1 || pos == -1) {
        return false;
    }
    arg1 = arg.substr(1, pos - 1);
    arg2 = arg.substr(pos + 1, len - pos - 1);
    return true;
}

std::vector<std::string> tokenize(const std::string& str) {
    std::vector<std::string> tokens;
    std::string token;
    for (const char ch : str) {
        if (ch == ' ') {
            if (ch == '\r') {
                continue;
            }
            if (!token.empty()) {
                tokens.push_back(token);
            }
            token.clear();
        }
        else {
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

int stoi_safe(const std::string &str) {
    if (str.size() > 10 || (str.size() > 1 && str[0] == '0')) {
        exit(14);
        throw std::invalid_argument("Invalid integer string");
    }
    // return std::stoi(str);
    std::istringstream iss(str);
    long long val;
    iss >> val;
    if (!iss.eof()) {
        iss >> std::ws;
    }
    if (iss.fail() || !iss.eof()) {
        throw std::invalid_argument("Invalid integer string");
    }
    if (val > 2147483647ll) {
        exit(13);
        throw std::out_of_range("Integer value out of range");
    }
    return int(val);
}

double stod_safe(const std::string &str) {
    if (str.size() > 13 || str[0] == '.' || (str.size() > 1 && str[0] == '0' && str[1] != '.') || (str.size() > 0 && str.back() == '.')) {
        exit(12);
        throw std::invalid_argument("Invalid double string");
    }
    // return std::stod(str);
    std::istringstream iss(str);
    double val;
    iss >> val;
    if (!iss.eof()) {
        iss >> std::ws;
    }
    if (iss.fail() || !iss.eof()) {
        throw std::invalid_argument("Invalid double string");
    }
    if (val > 1e11) {
        throw std::out_of_range("Double value out of range");
    }
    return val;
}

std::vector<char32_t> utf8_to_utf32(const std::string& str) {
    std::vector<char32_t> result;
    int i = 0;
    while (i < str.size()) {
        unsigned char ch = str[i];
        char32_t cp = 0;
        int len = 0;
        if (ch <= 0x7F) {
            cp = ch;
            len = 1;
        }
        else if ((ch & 0xE0) == 0xC0) {
            cp = ch & 0x1F;
            len = 2;
        }
        else if ((ch & 0xF0) == 0xE0) {
            cp = ch & 0x0F;
            len = 3;
        }
        else if ((ch & 0xF8) == 0xF0) {
            cp = ch & 0x07;
            len = 4;
        }
        else {
            throw "Invalid UTF-8 leading byte";
        }
        if (i + len > str.size()) {
            throw "Truncated UTF-8";
        }
        for (int j = 1; j < len; j++) {
            unsigned char cc = str[i + j];
            if ((cc & 0xC0) != 0x80) {
                throw "Invalid UTF-8 continuation byte";
            }
            cp = (cp << 6) | (cc & 0x3F);
        }
        result.push_back(cp);
        i += len;
    }
    return result;
}