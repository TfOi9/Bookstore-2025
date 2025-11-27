#ifndef FIXED_STRING_HPP
#define FIXED_STRING_HPP

#include <cstring>
#include <string>
#include <iostream>

template<typename CharType, int StringSize>
class FixedString {
private:
    CharType str_[StringSize];
public:
    FixedString() {
        memset(str_, 0, sizeof(str_));
    }
    FixedString(const char* str) {
        memset(str_, 0, sizeof(str_));
        const char* pos = str;
        int index = 0;
        while(*pos) {
            str_[index] = *pos;
            pos++;
            index++;
            if (index == StringSize) {
                return;
            }
        }
    }
    FixedString(const char* str, int size_) {
        memset(str_, 0, sizeof(str_));
        for (int i = 0; i < size_ && i < StringSize; i++) {
            str_[i] = str[i];
        }
    }
    FixedString(const std::string& str) {
        memset(str_, 0, sizeof(str_));
        for (int i = 0; i < str.size() && i < StringSize; i++) {
            str_[i] = str[i];
        }
    }
    bool operator==(const FixedString& oth) const {
        for (int i = 0; i < StringSize; i++) {
            if (str_[i] != oth.str_[i]) {
                return 0;
            }
        }
        return 1;
    }
    CharType operator[](int index) const {
        if (index < 0 || index >= StringSize) {
            return CharType();
        }
        return str_[index];
    }
    CharType& operator[](int index) {
        static CharType dummy = CharType();
        if (index < 0 || index >= StringSize) {
            return dummy;
        }
        return str_[index];
    }
    FixedString& operator=(const char* str) {
        memset(str_, 0, sizeof(str_));
        const char* pos = str;
        int index = 0;
        while(*pos) {
            str_[index] = *pos;
            pos++;
            index++;
            if (index == StringSize) {
                return *this;
            }
        }
        return *this;
    }
    FixedString& operator=(const std::string& str) {
        memset(str_, 0, sizeof(str_));
        for (int i = 0; i < str.size() && i < StringSize; i++) {
            str_[i] = str[i];
        }
        return *this;
    }
    FixedString& operator=(const FixedString& oth) {
        memcpy(str_, oth.str_, StringSize * sizeof(CharType));
        return *this;
    }
    CharType* serialize() const {
        CharType* str = new CharType[StringSize]();
        memcpy(str, str_, StringSize * sizeof(CharType));
        return str;
    }
    friend std::istream& operator>>(std::istream& is, FixedString& str) {
        std::string s;
        is >> s;
        str = s;
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const FixedString& str) {
        for (int i = 0; i < StringSize; i++) {
            if (str.str_[i]) {
                os << str.str_[i];
            }
            else {
                break;
            }
        }
        return os;
    }
};

#endif