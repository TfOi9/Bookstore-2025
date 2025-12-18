#include "../include/book.hpp"
#include "../include/utils.hpp"

std::vector<std::array<char, 240>> parse_keywords(const std::array<char, 240>& keyword) {
    std::vector<std::array<char, 240>> vec;
    int pos = 0;
    for (int i = 0; i <= 240; i++) {
        if (i == 240 || keyword[i] == '|' || keyword[i] == 0) {
            if (i == pos) {
                return std::vector<std::array<char, 240>>();
            }
            std::array<char, 240> arr;
            for (int j = 0; j < 240; j++) {
                arr[j] = 0;
            }
            for (int j = pos; j < i; j++) {
                arr[j - pos] = keyword[j];
            }
            vec.push_back(arr);
            pos = i + 1;
            if (i == 240 || keyword[i] == 0) {
                break;
            }
        }
    }
    return vec;
}

Book::Book(const std::string& ISBN, const std::string& book_name, const std::string& author, const std::string keyword, double price, int quant, int id) {
    ISBN_ = string_to_array<20>(ISBN);
    book_name_ = string_to_array<240>(book_name);
    author_ = string_to_array<240>(author);
    keyword_ = string_to_array<240>(keyword);
    price_ = price;
    quant_ = quant;
    id_ = id;
}

std::string Book::ISBN() const {
    return array_to_string<20>(ISBN_);
}

std::string Book::book_name() const {
    return array_to_string<240>(book_name_);
}

std::string Book::author() const {
    return array_to_string<240>(author_);
}

std::string Book::keyword() const {
    return array_to_string<240>(keyword_);
}

bool Book::operator<(const Book& oth) const {
    return ISBN() < oth.ISBN();
}

bool Book::operator==(const Book& oth) const {
    return (ISBN_ == oth.ISBN_ && book_name_ == oth.book_name_ && author_ == oth.author_ && keyword_ == oth.keyword_ && price_ == oth.price_ && quant_ == oth.quant_);
}

std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << book.ISBN() << '\t' << book.book_name() << '\t' << book.author() << '\t' << book.keyword() << '\t' << book.price_ << '\t' << book.quant_;
    return os;
}

BookManager::BookManager() : ISBN_file_("ISBN.dat"), book_name_file_("book_name.dat"), author_file_("author.dat"), keyword_file_("keyword.dat"), book_file_("book.dat") {
    book_file_.initialise();
}

BookManager::BookManager(const std::string& base_dir)
        : ISBN_file_(base_dir + "/ISBN.dat"),
            book_name_file_(base_dir + "/book_name.dat"),
            author_file_(base_dir + "/author.dat"),
            keyword_file_(base_dir + "/keyword.dat"),
            book_file_(base_dir + "/book.dat") {
    book_file_.initialise();
}

int BookManager::size() const {
    return book_file_.size();
}

Book BookManager::find(int id) {
    Book book;
    book_file_.read(book, id);
    return book;
}

std::vector<Book> BookManager::find_ISBN(const std::array<char, 20>& ISBN) {
    auto vec = ISBN_file_.find(ISBN);
    std::vector<Book> ret;
    for (int id : vec) {
        Book book;
        book_file_.read(book, id);
        ret.push_back(book);
    }
    return ret;
}

std::vector<Book> BookManager::find_book_name(const std::array<char, 240>& book_name) {
    auto vec = book_name_file_.find(book_name);
    std::vector<Book> ret;
    for (int id : vec) {
        Book book;
        book_file_.read(book, id);
        ret.push_back(book);
    }
    return ret;
}

std::vector<Book> BookManager::find_author(const std::array<char, 240>& author) {
    auto vec = author_file_.find(author);
    std::vector<Book> ret;
    for (int id : vec) {
        Book book;
        book_file_.read(book, id);
        ret.push_back(book);
    }
    return ret;
}

std::vector<Book> BookManager::find_keyword(const std::array<char, 240>& keyword) {
    auto vec = keyword_file_.find(keyword);
    std::vector<Book> ret;
    for (int id : vec) {
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
        if (book.ISBN_[0] == 0) {
            continue;
        }
        ret.push_back(book);
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}

void BookManager::add(Book& book) {
    book_file_.write(book);
    ISBN_file_.insert(book.ISBN_, book.id_);
    book_name_file_.insert(book.book_name_, book.id_);
    author_file_.insert(book.author_, book.id_);
    auto vec = parse_keywords(book.keyword_);
    for (auto kw : vec) {
        keyword_file_.insert(kw, book.id_);
    }
}

void BookManager::modify_ISBN(const std::array<char, 20>& ISBN, const std::array<char, 20>& new_ISBN) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec.empty()) {
        return;
    }
    int id = vec[0];
    Book book;
    book_file_.read(book, id);
    ISBN_file_.erase(ISBN, id);
    book.ISBN_ = new_ISBN;
    book_file_.update(book, id);
    ISBN_file_.insert(new_ISBN, id);
}

void BookManager::modify_book_name(const std::array<char, 20>& ISBN, const std::array<char, 240>& new_book_name) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec.empty()) {
        return;
    }
    int id = vec[0];
    Book book;
    book_file_.read(book, id);
    book_name_file_.erase(book.book_name_, id);
    book.book_name_ = new_book_name;
    book_file_.update(book, id);
    book_name_file_.insert(new_book_name, id);
}

void BookManager::modify_author(const std::array<char, 20>& ISBN, const std::array<char, 240>& new_author) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec.empty()) {
        return;
    }
    int id = vec[0];
    Book book;
    book_file_.read(book, id);
    author_file_.erase(book.author_, id);
    book.author_ = new_author;
    book_file_.update(book, id);
    author_file_.insert(new_author, id);
}

void BookManager::modify_keyword(const std::array<char, 20>& ISBN, const std::array<char, 240>& new_keyword) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec.empty()) {
        return;
    }
    int id = vec[0];
    Book book;
    book_file_.read(book, id);
    auto kws = parse_keywords(book.keyword_);
    for (auto kw : kws) {
        keyword_file_.erase(kw, id);
    }
    book.keyword_ = new_keyword;
    book_file_.update(book, id);
    kws = parse_keywords(new_keyword);
    for (auto kw : kws) {
        keyword_file_.insert(kw, id);
    }
}

void BookManager::modify_price(const std::array<char, 20>& ISBN, double new_price) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec.empty()) {
        return;
    }
    int id = vec[0];
    Book book;
    book_file_.read(book, id);
    book.price_ = new_price;
    book_file_.update(book, id);
}

bool BookManager::import(const std::array<char, 20>& ISBN, int d_quant) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec.empty()) {
        return false;
    }
    int id = vec[0];
    Book book;
    book_file_.read(book, id);
    book.quant_ += d_quant;
    book_file_.update(book, id);
    return true;
}

bool BookManager::buy(const std::array<char, 20>& ISBN, int quant, double& cost) {
    if (quant <= 0) {
        return false;
    }
    auto vec = ISBN_file_.find(ISBN);
    if (vec.empty()) {
        return false;
    }
    int id = vec[0];
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

int BookManager::count_ISBN(const std::array<char, 20>& ISBN) {
    return ISBN_file_.count(ISBN);
}

void BookManager::export_data(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for exporting data!");
    }
    auto books = serialize();
    file << "ISBN,Name,Author,Keyword,Price,Quantity\n";
    for (const auto& book : books) {
        file << "\"" << book.ISBN() << "\",\"" << book.book_name() << "\",\"" << book.author() << "\",\"" << book.keyword() << "\"," << book.price_ << ',' << book.quant_ << '\n';
    }
    file.close();
}

bool BookManager::delete_book(const std::array<char, 20>& ISBN) {
    auto vec = ISBN_file_.find(ISBN);
    if (vec.empty()) {
        return false;
    }
    int id = vec[0];
    Book book, empty_book;
    book_file_.read(book, id);
    book_file_.update(empty_book, id);
    ISBN_file_.erase(ISBN, id);
    book_name_file_.erase(book.book_name_, id);
    author_file_.erase(book.author_, id);
    auto kws = parse_keywords(book.keyword_);
    for (auto kw : kws) {
        keyword_file_.erase(kw, id);
    }
    return true;
}