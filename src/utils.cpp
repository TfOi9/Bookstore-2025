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