#ifndef BOOK_HPP
#define BOOK_HPP

#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "hash_map.hpp"
#include "memory_river.hpp"
#include "utils.hpp"

std::vector<std::array<char, 60>> parse_keywords(const std::array<char, 60>& keyword);

struct Book {
    std::array<char, 20> ISBN_;
    std::array<char, 60> book_name_;
    std::array<char, 60> author_;
    std::array<char, 60> keyword_;
    double price_;
    int quant_;
    int id_;
    Book(const std::string& ISBN = "", const std::string& book_name = "", const std::string& author = "", const std::string keyword = "", double price = 0.00, int quant = 0, int id = 0);

    ~Book() = default;

    std::string ISBN() const;

    std::string book_name() const;

    std::string author() const;

    std::string keyword() const;

    bool operator<(const Book& oth) const;

    bool operator==(const Book& oth) const;

    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};

class BookManager {
private:
    std::unordered_map<std::string, int> ISBN_file_;
    std::unordered_map<std::string, std::vector<int>> book_name_file_;
    std::unordered_map<std::string, std::vector<int>> author_file_;
    std::unordered_map<std::string, std::vector<int>> keyword_file_;
    MemoryRiver<Book> book_file_;

    void read_all();
public:
    BookManager();

    ~BookManager() = default;

    int size() const;

    Book find(int id);

    std::vector<Book> find_ISBN(const std::string& ISBN);

    std::vector<Book> find_book_name(const std::string& book_name);

    std::vector<Book> find_author(const std::string& author);

    std::vector<Book> find_keyword(const std::string& keyword);

    std::vector<Book> serialize();

    void add(Book& book);

    void modify_ISBN(const std::string& ISBN, const std::string& new_ISBN);

    void modify_book_name(const std::string& ISBN, const std::string& new_book_name);

    void modify_author(const std::string& ISBN, const std::string& new_author);

    void modify_keyword(const std::string& ISBN, const std::string& new_keyword);

    void modify_price(const std::string& ISBN, double new_price);

    bool import(const std::string& ISBN, int d_quant);

    bool buy(const std::string& ISBN, int quant, double& cost);

    int count_ISBN(const std::string& ISBN);

};

#endif