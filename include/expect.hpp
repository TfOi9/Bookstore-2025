#ifndef EXPECT_HPP
#define EXPECT_HPP

#include <string>
#include <type_traits>
#include <utility>
#include <regex>

template <typename T>
class Expect {
private:
    const T& val_;
    bool flipped_;
public:
    Expect(const T& val) : val_(val), flipped_(false) {}

    ~Expect() = default;

    Expect& Not() {
        flipped_ ^= 1;
        return *this;
    }

    Expect& toBe(const T& val) {
        bool flag = (val_ == val);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    // WARNING: toBe<U>() is a checker done in compilation. Previous Not() functions have no impact on it.
    template <typename U>
    Expect& toBe() {
        static_assert(std::is_base_of<T, U>::value || std::is_same<T, U>::value, "Expect failed.");
        if constexpr (std::is_polymorphic<T>::value) {
            bool flag = (dynamic_cast<const U*>(&val_) != nullptr);
            if (flipped_ ? flag : !flag) {
                throw "Expect failed.";
            }
        }
        return *this;
    }

    template<typename... Args>
    Expect& toBeOneOf(Args&&... args) {
        bool flag = ((val_ == std::forward<Args>(args)) || ...);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    Expect& le(const T& val) {
        bool flag = (val_ <= val);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    Expect& ge(const T& val) {
        bool flag = (val_ >= val);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    Expect& And() {
        return *this;
    }

    Expect& Or() {
        return *this;
    }

    Expect& But() {
        return *this;
    }
};

template<typename CharT, typename Traits, typename Alloc>
class Expect<std::basic_string<CharT, Traits, Alloc>> {
    using StringT = std::basic_string<CharT, Traits, Alloc>;
    const StringT& val_;
    bool flipped_;
public:
    Expect(const StringT& val) : val_(val), flipped_(false) {}

    ~Expect() = default;

    Expect& Not() {
        flipped_ ^= 1;
        return *this;
    }

    Expect& toBe(const StringT& val) {
        bool flag = (val_ == val);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    // WARNING: toBe<U>() is a checker done in compilation. Previous Not() functions have no impact on it.
    template <typename U>
    Expect& toBe() {
        static_assert(std::is_base_of<StringT, U>::value || std::is_same<StringT, U>::value, "Expect failed.");
        if constexpr (std::is_polymorphic<StringT>::value) {
            bool flag = (dynamic_cast<const U*>(&val_) != nullptr);
            if (flipped_ ? flag : !flag) {
                throw "Expect failed.";
            }
        }
        return *this;
    }

    template<typename... Args>
    Expect& toBeOneOf(Args&&... args) {
        bool flag = ((val_ == std::forward<Args>(args)) || ...);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    Expect& le(const StringT& val) {
        bool flag = (val_ <= val);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    Expect& ge(const StringT& val) {
        bool flag = (val_ >= val);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    Expect& And() {
        return *this;
    }

    Expect& Or() {
        return *this;
    }

    Expect& But() {
        return *this;
    }

    Expect& consistedOf(const StringT& str) {
        bool flag = true;
        for(CharT ch : val_) {
            if (str.find(ch) == StringT::npos) {
                flag = false;
                break;
            }
        }
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }

    Expect& toMatch(const StringT& str) {
        std::basic_regex<CharT> reg(str);
        bool flag = std::regex_match(val_, reg);
        if (flipped_ ? flag : !flag) {
            throw "Expect failed.";
        }
        return *this;
    }
};

#endif