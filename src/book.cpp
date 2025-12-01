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

bool Book::operator==(const Book& oth) const {
    return (ISBN_ == oth.ISBN_ && book_name_ == oth.book_name_ && author_ == oth.author_ && keyword_ == oth.keyword_ && price_ == oth.price_ && quant_ == oth.quant_);
}

BookManager::BookManager() : ISBN_file_("ISBN.dat"), book_name_file_("book.dat"), author_file_("author.dat"), keyword_file_("keyword.dat") , selected_book_() {}

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
    ISBN_file_.insert(new_book.ISBN_, new_book);
    book_name_file_.insert(new_book.book_name_, new_book);
    author_file_.insert(new_book.author_, new_book);
    keyword_file_.insert(new_book.keyword_, new_book);
    ISBN_file_.erase(book.ISBN_, book);
    book_name_file_.erase(book.book_name_, book);
    author_file_.erase(book.author_, book);
    keyword_file_.erase(book.keyword_, book);
    // std::vector<Book> test = ISBN_file_.find(arr);
    // std::cerr << "now sized " << test.size() << std::endl;
    return true;
}

void BookManager::select(const std::string& ISBN) {
    std::array<char, 20> arr = string_to_array<20>(ISBN);
    std::vector<Book> books = ISBN_file_.find(arr);
    has_selected_ = true;
    if (books.empty()) {
        Book book = Book(ISBN);
        ISBN_file_.insert(arr, book);
        selected_book_ = book;
        return;
    }
    selected_book_ = books[0];
}

bool BookManager::modify(const std::string& ISBN, const std::string& book_name, const std::string& author, const std::string& keyword, double price) {
    if (!has_selected_) {
        return false;
    }
    Book& book = selected_book_;
    if (ISBN != book.ISBN() && ISBN_file_.count(string_to_array<20>(ISBN))) {
        return false;
    }
    Book new_book = Book(ISBN, book_name, author, keyword, price, book.quant_);
    ISBN_file_.insert(new_book.ISBN_, new_book);
    book_name_file_.insert(new_book.book_name_, new_book);
    author_file_.insert(new_book.author_, new_book);
    keyword_file_.insert(new_book.keyword_, new_book);
    ISBN_file_.erase(book.ISBN_, book);
    book_name_file_.erase(book.book_name_, book);
    author_file_.erase(book.author_, book);
    keyword_file_.erase(book.keyword_, book);
    selected_book_ = new_book;
    return true;
}

bool BookManager::import(const std::string& ISBN, int quant) {
    if (quant <= 0) {
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
    // std::cerr << book.quant_ << " " << new_book.quant_ << std::endl;
    ISBN_file_.insert(new_book.ISBN_, new_book);
    book_name_file_.insert(new_book.book_name_, new_book);
    author_file_.insert(new_book.author_, new_book);
    keyword_file_.insert(new_book.keyword_, new_book);
    // std::cerr << "now delete old" << std::endl;
    ISBN_file_.erase(book.ISBN_, book);
    book_name_file_.erase(book.book_name_, book);
    author_file_.erase(book.author_, book);
    keyword_file_.erase(book.keyword_, book);
    // std::vector<Book> test = ISBN_file_.find(arr);
    // std::cerr << "now sized " << test.size() << std::endl;
    return true;
}

bool BookManager::import(const std::string& ISBN, double cost) {
    if (cost <= 0) {
        return false;
    }
    std::array<char, 20> arr = string_to_array<20>(ISBN);
    std::vector<Book> books = ISBN_file_.find(arr);
    if (books.empty()) {
        return false;
    }
    Book& book = books[0];
    int quant = cost / book.price_;
    Book new_book = book;
    new_book.quant_ += quant;
    ISBN_file_.insert(new_book.ISBN_, new_book);
    book_name_file_.insert(new_book.book_name_, new_book);
    author_file_.insert(new_book.author_, new_book);
    keyword_file_.insert(new_book.keyword_, new_book);
    ISBN_file_.erase(book.ISBN_, book);
    book_name_file_.erase(book.book_name_, book);
    author_file_.erase(book.author_, book);
    keyword_file_.erase(book.keyword_, book);
    return true;
}

void BookManager::load(const std::array<char, 20>& ISBN) {
    std::vector<Book> books = ISBN_file_.find(ISBN);
    has_selected_ = true;
    if (books.empty()) {
        return;
    }
    selected_book_ = books[0];
}

Book BookManager::selected_book() const {
    return selected_book_;
}