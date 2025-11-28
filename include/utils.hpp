#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <array>

template<int StringSize>
std::array<char, StringSize> string_to_array(const std::string& str);

template<int StringSize>
std::string array_to_string(const std::array<char, StringSize>& arr);

#endif