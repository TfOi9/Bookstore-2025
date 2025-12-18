#ifndef BOOK_HPP
#define BOOK_HPP

#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include "hash_map.hpp"
#include "memory_river.hpp"
#include "utils.hpp"

std::vector<std::array<char, 240>> parse_keywords(const std::array<char, 240>& keyword);

struct Book {
    std::array<char, 20> ISBN_;
    std::array<char, 240> book_name_;
    std::array<char, 240> author_;
    std::array<char, 240> keyword_;
    double price_;
    int quant_;
    int id_;
    Book(const std::string& ISBN = "", const std::string& book_name = "", const std::string& author = "", const std::string keyword = "", double price = 0.00, int quant = 0, int id = 0);

    ~Book() = default;

    // returns ISBN as string.
    std::string ISBN() const;

    // returns book name as string.
    std::string book_name() const;

    // returns author as string.
    std::string author() const;

    // returns keyword as string.
    std::string keyword() const;

    bool operator<(const Book& oth) const;

    bool operator==(const Book& oth) const;

    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};

class BookManager {
private:
    HashMap<std::array<char, 20>, int> ISBN_file_;
    HashMap<std::array<char, 240>, int> book_name_file_;
    HashMap<std::array<char, 240>, int> author_file_;
    HashMap<std::array<char, 240>, int> keyword_file_;
    MemoryRiver<Book> book_file_;
public:
    BookManager();

    BookManager(const std::string& base_dir);

    ~BookManager() = default;

    // returns the number of books.
    int size() const;

    // finds a book by id.
    Book find(int id);

    // finds books by ISBN.
    std::vector<Book> find_ISBN(const std::array<char, 20>& ISBN);

    // finds books by book name.
    std::vector<Book> find_book_name(const std::array<char, 240>& book_name);

    // finds books by author.
    std::vector<Book> find_author(const std::array<char, 240>& author);

    // finds books by keyword.
    std::vector<Book> find_keyword(const std::array<char, 240>& keyword);

    // serializes all books.
    std::vector<Book> serialize();

    // adds a new book.
    void add(Book& book);

    // modifies book's ISBN.
    void modify_ISBN(const std::array<char, 20>& ISBN, const std::array<char, 20>& new_ISBN);

    // modifies book's name.
    void modify_book_name(const std::array<char, 20>& ISBN, const std::array<char, 240>& new_book_name);

    // modifies book's author.
    void modify_author(const std::array<char, 20>& ISBN, const std::array<char, 240>& new_author);

    // modifies book's keyword.
    void modify_keyword(const std::array<char, 20>& ISBN, const std::array<char, 240>& new_keyword);

    // modifies book's price.
    void modify_price(const std::array<char, 20>& ISBN, double new_price);

    // imports books.
    bool import(const std::array<char, 20>& ISBN, int d_quant);

    // buys books.
    bool buy(const std::array<char, 20>& ISBN, int quant, double& cost);

    // counts book by ISBN.
    int count_ISBN(const std::array<char, 20>& ISBN);

    // exports data to a CSV file.
    void export_data(const std::string& filename = "book.csv");

    // deletes a book.
    bool delete_book(const std::array<char, 20>& ISBN);
};

#endif