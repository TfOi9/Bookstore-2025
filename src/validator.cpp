#include "../include/validator.hpp"

Validator::Validator(const std::string& str, bool valid) : str_(str), valid_(valid) {}

Validator& Validator::max_len(int len) {
    if (str_.size() > len) {
        valid_ = false;
    }
    return *this;
}

Validator& Validator::min_len(int len) {
    if (str_.size() < len) {
        valid_ = false;
    }
    return *this;
}

Validator& Validator::visible_only() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < str_.size(); i++) {
        if (str_[i] < 33 || str_[i] > 126) {
            valid_ = false;
            break;
        }
    }
    return *this;
}

Validator& Validator::normal_char_only() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < str_.size(); i++) {
        char ch = str_[i];
        bool is_number = (ch >= '0' && ch <= '9');
        bool is_letter = (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z');
        bool is_underline = (ch == '_');
        if (is_number == 0 && is_letter == 0 && is_underline == 0) {
            valid_ = false;
            break;
        }
    }
    return *this;
}

Validator& Validator::no_quotes() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < str_.size(); i++) {
        if (str_[i] == '"') {
            valid_ = false;
            break;
        }
    }
    return *this;
}

Validator& Validator::number_only() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < str_.size(); i++) {
        if (str_[i] < '0' || str_[i] > '9') {
            valid_ = false;
            break;
        }
    }
    return *this;
}

Validator& Validator::number_and_dot_only() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < str_.size(); i++) {
        if ((str_[i] < '0' || str_[i] > '9') && str_[i] != '.') {
            if (i == 0 && str_[i] == '+') {
                continue;
            }
            valid_ = false;
            break;
        }
    }
    return *this;
}

Validator& Validator::only_one_dot() {
    if (!valid_) {
        return *this;
    }
    bool flag = 0;
    for (int i = 0; i < str_.size(); i++) {
        if (str_[i] == '.') {
            if (flag) {
                valid_ = 0;
                break;
            }
            else {
                flag = 1;
            }
        }
    }
    return *this;
}

Validator& Validator::no_pipes() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < str_.size(); i++) {
        if (str_[i] == '|') {
            valid_ = 0;
            break;
        }
    }
    return *this;
}

Validator::operator bool() const {
    return valid_;
}