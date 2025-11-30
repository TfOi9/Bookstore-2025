#include "../include/book.hpp"

Book::Book(const std::string& ISBN, const std::string& book_name, const std::string& author, const std::string keyword, double price, int quant) {
    ISBN_ = string_to_array<20>(ISBN);
    book_name_ = string_to_array<60>(book_name);
    author_ = string_to_array<60>(author);
    keyword_ = string_to_array<60>(keyword);
    price_ = price;
    quant_ = quant;
}

std::string Book::ISBN() const {
    return array_to_string<20>(ISBN_);
}

std::string Book::book_name() const {
    return array_to_string<60>(book_name_);
}

std::string Book::author() const {
    return array_to_string<60>(author_);
}

std::string Book::keyword() const {
    return array_to_string<60>(keyword_);
}

double Book::price() const {
    return price_;
}

int Book::quant() const {
    return quant_;
}