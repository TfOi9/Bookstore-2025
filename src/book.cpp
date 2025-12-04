#include "../include/book.hpp"
#include "utils.hpp"

std::vector<std::array<char, 60>> parse_keywords(const std::array<char, 60>& keyword) {
    std::vector<std::array<char, 60>> vec;
    int pos = 0;
    for (int i = 1; i < 60; i++) {
        if (keyword[i] == '|' || i == 59 || keyword[i] == 0) {
            std::array<char, 60> arr;
            for (int j = 0; j < 60; j++) {
                arr[j] = 0;
            }
            for (int j = pos; j < i; j++) {
                arr[j - pos] = keyword[j];
            }
            vec.push_back(arr);
            pos = i + 1;
            if (keyword[i] == 0) {
                break;
            }
        }
    }
    return vec;
}

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

bool Book::operator<(const Book& oth) const {
    return ISBN_ < oth.ISBN_;
}

bool Book::operator==(const Book& oth) const {
    return (ISBN_ == oth.ISBN_ && book_name_ == oth.book_name_ && author_ == oth.author_ && keyword_ == oth.keyword_ && price_ == oth.price_ && quant_ == oth.quant_);
}

std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << book.ISBN() << '\t' << book.book_name() << '\t' << book.author() << '\t' << book.keyword() << '\t' << book.price() << '\t' << book.quant();
    return os;
}

void BookManager::update_book(const Book& book, const Book& new_book) {
    ISBN_file_.erase(book.ISBN_, book);
    book_name_file_.erase(book.book_name_, book);
    author_file_.erase(book.author_, book);
    auto vec = parse_keywords(book.keyword_);
    for (auto kw : vec) {
        keyword_file_.erase(kw, book);
    }
    ISBN_file_.insert(new_book.ISBN_, new_book);
    book_name_file_.insert(new_book.book_name_, new_book);
    author_file_.insert(new_book.author_, new_book);
    vec = parse_keywords(new_book.keyword_);
    for (auto kw : vec) {
        std::cerr << array_to_string<60>(kw) << std::endl;
        keyword_file_.insert(kw, new_book);
    }
}

BookManager::BookManager() : ISBN_file_("ISBN.dat"), book_name_file_("book.dat"), author_file_("author.dat"), keyword_file_("keyword.dat") {}

bool BookManager::add(const std::string& ISBN) {
    if (ISBN_file_.count(string_to_array<20>(ISBN))) {
        return false;
    }
    ISBN_file_.insert(string_to_array<20>(ISBN), Book(ISBN));
    return true;
}

std::vector<Book> BookManager::find_ISBN(const std::string& ISBN) {
    std::vector<Book> vec = ISBN_file_.find(string_to_array<20>(ISBN));
    std::sort(vec.begin(), vec.end());
    return vec;
}

std::vector<Book> BookManager::find_book_name(const std::string& book_name) {
    std::vector<Book> vec = book_name_file_.find(string_to_array<60>(book_name));
    std::sort(vec.begin(), vec.end());
    return vec;
}

std::vector<Book> BookManager::find_author(const std::string& author) {
    std::vector<Book> vec = author_file_.find(string_to_array<60>(author));
    std::sort(vec.begin(), vec.end());
    return vec;
}

std::vector<Book> BookManager::find_keyword(const std::string& keyword) {
    std::vector<Book> vec = keyword_file_.find(string_to_array<60>(keyword));
    std::sort(vec.begin(), vec.end());
    return vec;
}

std::vector<Book> BookManager::find_all() {
    std::vector<Book> vec = ISBN_file_.serialize();
    std::sort(vec.begin(), vec.end());
    return vec;
}

bool BookManager::buy(const std::string& ISBN, int quant, double& cost) {
    if (quant <= 0) {
        return false;
    }
    std::array<char, 20> arr = string_to_array<20>(ISBN);
    std::vector<Book> books = ISBN_file_.find(arr);
    if (books.empty()) {
        // std::cerr << "not found" << std::endl;
        return false;
    }
    // std::cerr << "sized " << books.size() << std::endl;
    Book& book = books[0];
    // std::cerr << book.quant_ << std::endl;
    if (book.quant_ < quant) {
        return false;
    }
    cost = book.price_ * quant;
    Book new_book = book;
    new_book.quant_ -= quant;
    update_book(book, new_book);
    return true;
}

bool BookManager::modify_ISBN(const std::string& ISBN, const std::string& new_ISBN) {
    if (ISBN == new_ISBN) {
        return false;
    }
    if (ISBN_file_.count(string_to_array<20>(new_ISBN))) {
        return false;
    }
    std::vector<Book> books = ISBN_file_.find(string_to_array<20>(ISBN));
    if (books.empty()) {
        return false;
    }
    Book& book = books[0];
    Book new_book = book;
    new_book.ISBN_ = string_to_array<20>(new_ISBN);
    update_book(book, new_book);
    return true;
}

bool BookManager::modify_book_name(const std::string& ISBN, const std::string& new_book_name) {
    std::vector<Book> books = ISBN_file_.find(string_to_array<20>(ISBN));
    if (books.empty()) {
        return false;
    }
    Book& book = books[0];
    Book new_book = book;
    new_book.book_name_ = string_to_array<60>(new_book_name);
    update_book(book, new_book);
    return true;
}

bool BookManager::modify_author(const std::string& ISBN, const std::string& new_author) {
    std::vector<Book> books = ISBN_file_.find(string_to_array<20>(ISBN));
    if (books.empty()) {
        return false;
    }
    Book& book = books[0];
    Book new_book = book;
    new_book.author_ = string_to_array<60>(new_author);
    update_book(book, new_book);
    return true;
}

bool BookManager::modify_keyword(const std::string& ISBN, const std::string& new_keyword) {
    std::vector<Book> books = ISBN_file_.find(string_to_array<20>(ISBN));
    if (books.empty()) {
        return false;
    }
    auto key_words = parse_keywords(string_to_array<60>(new_keyword));
    std::sort(key_words.begin(), key_words.end());
    if (std::unique(key_words.begin(), key_words.end()) != key_words.end()) {
        return false;
    }
    Book& book = books[0];
    Book new_book = book;
    new_book.keyword_ = string_to_array<60>(new_keyword);
    update_book(book, new_book);
    return true;
}

bool BookManager::modify_price(const std::string& ISBN, double new_price) {
    std::vector<Book> books = ISBN_file_.find(string_to_array<20>(ISBN));
    if (books.empty()) {
        return false;
    }
    Book& book = books[0];
    Book new_book = book;
    new_book.price_ = new_price;
    update_book(book, new_book);
    return true;
}

bool BookManager::import(const std::string& ISBN, int quant, double cost) {
    if (quant <= 0 || cost <= 0.00) {
        return false;
    }
    std::array<char, 20> arr = string_to_array<20>(ISBN);
    std::vector<Book> books = ISBN_file_.find(arr);
    if (books.empty()) {
        return false;
    }
    Book& book = books[0];
    Book new_book = book;
    new_book.quant_ += quant;
    update_book(book, new_book);
    return true;
}