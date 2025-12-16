#ifndef ACCOUNT_TABLE_HPP
#define ACCOUNT_TABLE_HPP

#include "qt_common.hpp"
#include "globals.hpp"

class AccountTable : public QTableWidget {
    Q_OBJECT
public:
    AccountTable(QWidget* parent = nullptr);

    void refreshTable();

};

#endif