#ifndef BOOK_TABLE_HPP
#define BOOK_TABLE_HPP

#include "qt_common.hpp"
#include "globals.hpp"

class BookTable : public QTableWidget {
    Q_OBJECT
public:
    BookTable(QWidget* parent = nullptr);

    void refreshTable();

};

#endif