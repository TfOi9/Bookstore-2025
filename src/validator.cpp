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

bool is_ascii(char32_t cp) {
    return cp <= 0x7F;
}

bool is_visible_ascii(char32_t cp) {
    return cp >= 33 && cp <= 126;
}

bool is_han(char32_t cp) {
    return
        (cp >= 0x4E00 && cp <= 0x9FFF) ||       // CJK Unified Ideographs
        (cp >= 0x3400 && cp <= 0x4DBF) ||       // CJK Unified Ideographs Extension A
        (cp >= 0x20000 && cp <= 0x2A6DF) ||     // Extension B
        (cp >= 0x2A700 && cp <= 0x2B73F) ||     // Extension C
        (cp >= 0x2B740 && cp <= 0x2B81F) ||     // Extension D
        (cp >= 0x2B820 && cp <= 0x2CEAF) ||     // Extension E
        (cp >= 0x2CEB0 && cp <= 0x2EBEF) ||     // Extension F
        (cp >= 0x30000 && cp <= 0x3134F);       // Extension G
}

bool is_special(char32_t cp) {
    return
        (cp == 0x00B7) ||       // 「·」
        (cp == 0x2014) ||       // 「—」
        (cp == 0xFF08) ||       // 「（」
        (cp == 0xFF09) ||       // 「）」
        (cp == 0xFF1A);         // 「：」
}

ChineseValidator::ChineseValidator(const std::string& str, bool valid) : utf32_str_(utf8_to_utf32(str)), valid_(valid) {}

ChineseValidator& ChineseValidator::max_len(int len) {
    if (utf32_str_.size() > len) {
        valid_ = false;
    }
    return *this;
}

ChineseValidator& ChineseValidator::min_len(int len) {
    if (utf32_str_.size() < len) {
        valid_ = false;
    }
    return *this;
}

ChineseValidator& ChineseValidator::visible_only() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < utf32_str_.size(); i++) {
        char32_t cp = utf32_str_[i];
        if (cp < 33 || cp > 126) {
            valid_ = false;
            break;
        }
    }
    return *this;
}

ChineseValidator& ChineseValidator::normal_char_only() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < utf32_str_.size(); i++) {
        char32_t cp = utf32_str_[i];
        bool is_number = (cp >= '0' && cp <= '9');
        bool is_letter = (cp >= 'A' && cp <= 'Z' || cp >= 'a' && cp <= 'z');
        bool is_underline = (cp == '_');
        if (is_number == 0 && is_letter == 0 && is_underline == 0) {
            valid_ = false;
            break;
        }
    }
    return *this;
}

ChineseValidator& ChineseValidator::no_quotes() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < utf32_str_.size(); i++) {
        if (utf32_str_[i] == '"') {
            valid_ = false;
            break;
        }
    }
    return *this;
}

ChineseValidator& ChineseValidator::number_only() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < utf32_str_.size(); i++) {
        if (utf32_str_[i] < '0' || utf32_str_[i] > '9') {
            valid_ = false;
            break;
        }
    }
    return *this;
}

ChineseValidator& ChineseValidator::number_and_dot_only() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < utf32_str_.size(); i++) {
        if ((utf32_str_[i] < '0' || utf32_str_[i] > '9') && utf32_str_[i] != '.') {
            valid_ = false;
            break;
        }
    }
    return *this;
}

ChineseValidator& ChineseValidator::only_one_dot() {
    if (!valid_) {
        return *this;
    }
    bool flag = 0;
    for (int i = 0; i < utf32_str_.size(); i++) {
        if (utf32_str_[i] == '.') {
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

ChineseValidator& ChineseValidator::no_pipes() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < utf32_str_.size(); i++) {
        if (utf32_str_[i] == '|') {
            valid_ = 0;
            break;
        }
    }
    return *this;
}

ChineseValidator& ChineseValidator::han() {
    if (!valid_) {
        return *this;
    }
    for (int i = 0; i < utf32_str_.size(); i++) {
        if (!is_visible_ascii(utf32_str_[i]) && !is_han(utf32_str_[i]) && !is_special(utf32_str_[i])) {
            valid_ = false;
            break;
        }
    }
    return *this;
}

ChineseValidator::operator bool() const {
    return valid_;
}