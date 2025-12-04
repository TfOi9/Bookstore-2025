#ifndef BOOK_HPP
#define BOOK_HPP

#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include "hash_map.hpp"
#include "utils.hpp"

std::vector<std::array<char, 60>> parse_keywords(const std::array<char, 60>& keyword);

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
    Book(const std::string& ISBN = "", const std::string& book_name = "", const std::string& author = "", const std::string keyword = "", double price = 0.00, int quant = 0);

    ~Book() = default;

    std::string ISBN() const;

    std::string book_name() const;

    std::string author() const;

    std::string keyword() const;

    double price() const;

    int quant() const;

    void set_ISBN(const std::string& new_ISBN);

    void set_book_name(const std::string& new_book_name);

    void set_author(const std::string& new_author);

    void set_keyword(const std::string& new_keyword);

    void set_price(double new_price);

    bool operator<(const Book& oth) const;

    bool operator==(const Book& oth) const;

    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};

class BookManager {
private:
    HashMap<std::array<char, 20>, Book> ISBN_file_;
    HashMap<std::array<char, 60>, Book> book_name_file_;
    HashMap<std::array<char, 60>, Book> author_file_;
    HashMap<std::array<char, 60>, Book> keyword_file_;
    // Book selected_book_;
    // bool has_selected_ = false;
    void update_book(const Book& book, const Book& new_book);
public:
    BookManager();

    ~BookManager() = default;

    bool add(const std::string& ISBN);

    int count(const std::string& ISBN);

    Book find(const std::string& ISBN);

    std::vector<Book> find_ISBN(const std::string& ISBN);

    std::vector<Book> find_book_name(const std::string& book_name);

    std::vector<Book> find_author(const std::string& author);

    std::vector<Book> find_keyword(const std::string& keyword);

    std::vector<Book> find_all();

    bool buy(const std::string& ISBN, int quant, double& cost);

    bool modify_ISBN(const std::string& ISBN, const std::string& new_ISBN);

    bool modify_book_name(const std::string& ISBN, const std::string& new_name);

    bool modify_author(const std::string& ISBN, const std::string& new_author);

    bool modify_keyword(const std::string& ISBN, const std::string& new_keyword);

    bool modify_price(const std::string& ISBN, double new_price);

    bool modify(const std::string& ISBN, const std::string& new_ISBN, const std::string& new_name, const std::string& new_author, const std::string& new_keyword, double new_price);
    
    bool modify(const std::string& ISBN, const Book& new_book);

    bool import(const std::string& ISBN, int quant, double cost);
};

#endif