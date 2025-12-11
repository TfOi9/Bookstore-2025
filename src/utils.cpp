#include "../include/utils.hpp"

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
    if (pos >= len - 3) {
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
    if (pos >= len - 1) {
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
        throw std::invalid_argument("Invalid integer string");
    }
    return std::stoi(str);
}

double stod_safe(const std::string &str) {
    if (str.size() > 13 || str[0] == '.' || (str.size() > 1 && str[0] == '0' && str[1] != '.')) {
        throw std::invalid_argument("Invalid double string");
    }
    return std::stod(str);
}