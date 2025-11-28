#ifndef BOOK_HPP
#define BOOK_HPP

#include <array>
#include <string>
#include <vector>
#include "hash_map.hpp"
#include "utils.hpp"

class Book {
    friend class BookManager;
private:
    std::array<char, 20> ISBN_;
    std::array<char, 60> book_name_;
    std::array<char, 60> author_;
    std::array<char, 60> keyword_;
    double price_;
    int quant_;
public:
    Book(const std::string& ISBN = "", const std::string& book_name = "", const std::string& author = "", const std::string keyword = "", double price = 0.00, int quant = 0) {}

    ~Book() = default;

    std::string ISBN() const;

    std::string book_name() const;

    std::string author() const;

    std::string keyword() const;

    double price() const;

    int quant() const;
};

class BookManager {
private:
    HashMap<std::array<char, 20>, Book> book_file_;
    Book selected_book_;
public:
    BookManager(const std::string& file_name = "book.dat") {}

    ~BookManager() = default;

    std::vector<Book> find(const std::string& ISBN, const std::string& book_name, const std::string& author, const std::string& keyword);

    bool buy(const std::string& ISBN, int quant);

    bool select(const std::string& ISBN);

    std::vector<Book> modify(const std::string& ISBN, const std::string& book_name, const std::string& author, const std::string& keyword, int price);

    bool import(const std::string ISBN, int quant);
};

#endif