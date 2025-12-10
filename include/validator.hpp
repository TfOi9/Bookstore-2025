#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <iostream>
#include <string>

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

#endif