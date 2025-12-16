#include "book_table.hpp"
#include "globals.hpp"
#include <QDateTime>
#include <QtCore/qlogging.h>

BookTable::BookTable(QWidget* parent) : QTableWidget(parent) {
    setColumnCount(6);
    QStringList headers = {"ISBN", "书名", "作者", "关键词", "价格", "库存"};
    setHorizontalHeaderLabels(headers);
    setColumnWidth(0, 150);
    setColumnWidth(1, 150);
    setColumnWidth(2, 150);
    setColumnWidth(3, 150);
    setColumnWidth(4, 99);
    setColumnWidth(5, 99);
    verticalHeader()->setVisible(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this, &QTableWidget::itemSelectionChanged, this, &BookTable::handleSelectionChanged);
    books_ = book_manager->serialize();
}

void BookTable::refreshTable(bool update) {
    clearContents();
    if (account_manager->current_privilege() < 1) {
        setRowCount(0);
        return;
    }
    std::vector<Book> books;
    if (update) books = book_manager->serialize();
    else books = books_;
    books_ = books;
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

void BookTable::handleSelectionChanged() {
    auto items = selectedItems();
    if (items.isEmpty()) {
        return;
    }
    int row = items.first()->row();
    QTableWidgetItem* ISBNItem = item(row, 0);
    if (!ISBNItem) return;
    QString ISBN = ISBNItem->text();
    emit bookSelected(ISBN);
}

void BookTable::updateBooks(const std::vector<Book>& books) {
    qDebug() << "Updating books in BookTable, new size:" << books.size();
    books_ = books;
    refreshTable();
}

void BookTable::handleSearch(const std::vector<Book>& books) {
    qDebug() << "Handling search in BookTable, result size:" << books.size();
    books_ = books;
    refreshTable(0);
}