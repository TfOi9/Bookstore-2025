#include "book_table.hpp"

BookTable::BookTable(QWidget* parent) : QTableWidget(parent) {
    setColumnCount(6);
    QStringList headers = {"ISBN", "书名", "作者", "关键词", "价格", "库存"};
    setHorizontalHeaderLabels(headers);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setVisible(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void BookTable::refreshList() {
    clearContents();
    const auto& books = book_manager->serialize();
    setRowCount(books.size());
    for (int i = 0; i < books.size(); i++) {
        const auto& book = books[i];
        setItem(i, 0, new QTableWidgetItem(QString::fromStdString(book.ISBN())));
        setItem(i, 1, new QTableWidgetItem(QString::fromStdString(book.book_name())));
            setItem(i, 2, new QTableWidgetItem(QString::fromStdString(book.author())));
            setItem(i, 3, new QTableWidgetItem(QString::fromStdString(book.keyword())));
            setItem(i, 4, new QTableWidgetItem(QString::number(book.price_, 'f', 2)));
            setItem(i, 5, new QTableWidgetItem(QString::number(book.quant_)));
        }
        resizeRowsToContents();
    }