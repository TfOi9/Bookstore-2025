#include "../include/cached_book.hpp"
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

Book::Book(const std::string& ISBN, const std::string& book_name, const std::string& author, const std::string keyword, double price, int quant, int id) {
    ISBN_ = string_to_array<20>(ISBN);
    book_name_ = string_to_array<60>(book_name);
    author_ = string_to_array<60>(author);
    keyword_ = string_to_array<60>(keyword);
    price_ = price;
    quant_ = quant;
    id_ = id;
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

bool Book::operator<(const Book& oth) const {
    return ISBN_ < oth.ISBN_;
}

bool Book::operator==(const Book& oth) const {
    return (ISBN_ == oth.ISBN_ && book_name_ == oth.book_name_ && author_ == oth.author_ && keyword_ == oth.keyword_ && price_ == oth.price_ && quant_ == oth.quant_);
}

std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << book.ISBN() << '\t' << book.book_name() << '\t' << book.author() << '\t' << book.keyword() << '\t' << book.price_ << '\t' << book.quant_;
    return os;
}

void BookManager::read_all() {
    int len = book_file_.size();
    for (int i = 0; i < len; i++) {
        Book book;
        book_file_.read(book, i);
        ISBN_file_[book.ISBN()] = i;
        book_name_file_[book.book_name()].push_back(i);
        author_file_[book.author()].push_back(i);
        auto kws = parse_keywords(book.keyword_);
        for (auto kw : kws) {
            std::string skw = array_to_string<60>(kw);
            keyword_file_[skw].push_back(i);
        }
    }
}

BookManager::BookManager() : book_file_("book.dat") {
    book_file_.initialise();
    read_all();
}

int BookManager::size() const {
    return book_file_.size();
}

Book BookManager::find(int id) {
    Book book;
    book_file_.read(book, id);
    return book;
}

std::vector<Book> BookManager::find_ISBN(const std::string& ISBN) {
    if (ISBN_file_.find(ISBN) == ISBN_file_.end()) {
        return std::vector<Book>();
    }
    int id = ISBN_file_[ISBN];
    Book book;
    book_file_.read(book, id);
    return std::vector<Book>{book};
}

std::vector<Book> BookManager::find_book_name(const std::string& book_name) {
    auto vec = book_name_file_.find(book_name);
    std::vector<Book> ret;
    if (vec == book_name_file_.end()) {
        return ret;
    }
    for (int id : vec->second) {
        Book book;
        book_file_.read(book, id);
        ret.push_back(book);
    }
    return ret;
}

std::vector<Book> BookManager::find_author(const std::string& author) {
    auto vec = author_file_.find(author);
    std::vector<Book> ret;
    if (vec == author_file_.end()) {
        return ret;
    }
    for (int id : vec->second) {
        Book book;
        book_file_.read(book, id);
        ret.push_back(book);
    }
    return ret;
}

std::vector<Book> BookManager::find_keyword(const std::string& keyword) {
    auto vec = keyword_file_.find(keyword);
    std::vector<Book> ret;
    if (vec == keyword_file_.end()) {
        return ret;
    }
    for (int id : vec->second) {
        Book book;
        book_file_.read(book, id);
        ret.push_back(book);
    }
    return ret;
}

std::vector<Book> BookManager::serialize() {
    std::vector<Book> ret;
    for (int i = 0; i < book_file_.size(); i++) {
        Book book;
        book_file_.read(book, i);
        ret.push_back(book);
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}

void BookManager::add(Book& book) {
    book_file_.write(book);
    ISBN_file_[book.ISBN()] = book.id_;
    book_name_file_[book.book_name()].push_back(book.id_);
    author_file_[book.author()].push_back(book.id_);
    auto vec = parse_keywords(book.keyword_);
    for (auto kw : vec) {
        std::string skw = array_to_string<60>(kw);
        keyword_file_[skw].push_back(book.id_);
    }
}

void BookManager::modify_ISBN(const std::string& ISBN, const std::string& new_ISBN) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec == ISBN_file_.end()) {
        return;
    }
    int id = vec->second;
    Book book;
    book_file_.read(book, id);
    ISBN_file_.erase(vec);
    book.ISBN_ = string_to_array<20>(new_ISBN);
    book_file_.update(book, id);
    ISBN_file_[new_ISBN] = id;
}

void BookManager::modify_book_name(const std::string& ISBN, const std::string& new_book_name) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec == ISBN_file_.end()) {
        return;
    }
    int id = vec->second;
    Book book;
    book_file_.read(book, id);
    auto it = book_name_file_.find(book.book_name());
    vector_remove(it->second, id);
    book.book_name_ = string_to_array<60>(new_book_name);
    book_file_.update(book, id);
    book_name_file_[new_book_name].push_back(id);
}

void BookManager::modify_author(const std::string& ISBN, const std::string& new_author) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec == ISBN_file_.end()) {
        return;
    }
    int id = vec->second;
    Book book;
    book_file_.read(book, id);
    auto it = author_file_.find(book.author());
    vector_remove(it->second, id);
    book.author_ = string_to_array<60>(new_author);
    book_file_.update(book, id);
    author_file_[new_author].push_back(id);
}

void BookManager::modify_keyword(const std::string& ISBN, const std::string& new_keyword) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec == ISBN_file_.end()) {
        return;
    }
    int id = vec->second;
    Book book;
    book_file_.read(book, id);
    auto kws = parse_keywords(book.keyword_);
    for (auto kw : kws) {
        std::string skw = array_to_string<60>(kw);
        auto it = keyword_file_.find(skw);
        vector_remove(it->second, id);
    }
    book.keyword_ = string_to_array<60>(new_keyword);
    book_file_.update(book, id);
    kws = parse_keywords(book.keyword_);
    for (auto kw : kws) {
        std::string skw = array_to_string<60>(kw);
        keyword_file_[skw].push_back(id);
    }
}

void BookManager::modify_price(const std::string& ISBN, double new_price) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec == ISBN_file_.end()) {
        return;
    }
    int id = vec->second;
    Book book;
    book_file_.read(book, id);
    book.price_ = new_price;
    book_file_.update(book, id);
}

bool BookManager::import(const std::string& ISBN, int d_quant) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec == ISBN_file_.end()) {
        return false;
    }
    int id = vec->second;
    Book book;
    book_file_.read(book, id);
    book.quant_ += d_quant;
    book_file_.update(book, id);
    return true;
}

bool BookManager::buy(const std::string& ISBN, int quant, double& cost) {
    if (quant <= 0) {
        return false;
    }
    auto vec = ISBN_file_.find(ISBN);
    if (vec == ISBN_file_.end()) {
        return false;
    }
    int id = vec->second;
    Book book;
    book_file_.read(book, id);
    if (book.quant_ < quant) {
        return false;
    }
    book.quant_ -= quant;
    cost = book.price_ * quant;
    book_file_.update(book, id);
    return true;
}

int BookManager::count_ISBN(const std::string& ISBN) {
    return ISBN_file_.count(ISBN);
}