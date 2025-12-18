#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include "utils.hpp"

class Validator {
private:
    std::string str_;
    bool valid_;
public:
    Validator(const std::string& str, bool valid = true);

    ~Validator() = default;

    Validator& max_len(int len);

    Validator& min_len(int len);

    Validator& visible_only();

    Validator& normal_char_only();

    Validator& no_quotes();

    Validator& number_only();

    Validator& number_and_dot_only();

    Validator& only_one_dot();

    Validator& no_pipes();

    operator bool() const;
};

bool is_ascii(char32_t cp);

bool is_visible_ascii(char32_t cp);

bool is_han(char32_t cp);

bool is_special(char32_t cp);

class ChineseValidator {
private:
    std::vector<char32_t> utf32_str_;
    bool valid_;
public:
    ChineseValidator(const std::string& str, bool valid = true);

    ~ChineseValidator() = default;

    ChineseValidator& max_len(int len);

    ChineseValidator& min_len(int len);

    ChineseValidator& visible_only();

    ChineseValidator& normal_char_only();

    ChineseValidator& no_quotes();

    ChineseValidator& number_only();

    ChineseValidator& number_and_dot_only();

    ChineseValidator& only_one_dot();

    ChineseValidator& no_pipes();

    ChineseValidator& han();

    operator bool() const;
};

#endif