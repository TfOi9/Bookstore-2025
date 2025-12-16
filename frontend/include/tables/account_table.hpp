#ifndef ACCOUNT_TABLE_HPP
#define ACCOUNT_TABLE_HPP

#include "qt_common.hpp"
#include "globals.hpp"

class AccountTable : public QTableWidget {
    Q_OBJECT
public:
    AccountTable(QWidget* parent = nullptr);

    void refreshTable(bool update = 1);

    void handleSearch(const std::vector<Account>& accounts);

signals:
    void accountSelected(const QString& user_id);

public slots:
    void updateAccounts(const std::vector<Account>& accounts);

private slots:
    void handleSelectionChanged();

private:
    std::vector<Account> accounts_;

};

#endif