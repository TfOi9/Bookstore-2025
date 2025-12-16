#ifndef BOOK_TABLE_HPP
#define BOOK_TABLE_HPP

#include <vector>
#include "qt_common.hpp"
#include "globals.hpp"

class BookTable : public QTableWidget {
    Q_OBJECT
public:
    BookTable(QWidget* parent = nullptr);

    void refreshTable(bool update = 1);

    void handleSearch(const std::vector<Book>& books);

signals:
    void bookSelected(const QString& ISBN);

public slots:
    void updateBooks(const std::vector<Book>& books);

private slots:
    void handleSelectionChanged();

private:
    std::vector<Book> books_;

};

#endif