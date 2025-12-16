#include "account_table.hpp"
#include "account.hpp"
#include "globals.hpp"

AccountTable::AccountTable(QWidget* parent) : QTableWidget(parent) {
    setColumnCount(3);
    QStringList headers = {"用户ID", "用户名", "权限"};
    setHorizontalHeaderLabels(headers);
    setColumnWidth(0, 350);
    setColumnWidth(1, 350);
    setColumnWidth(2, 98);
    verticalHeader()->setVisible(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void AccountTable::refreshTable(bool update) {
    clearContents();
    if (account_manager->current_privilege() < 3) {
        setRowCount(0);
        return;
    }
    std::vector<Account> accounts;
    if (update) accounts = account_manager->serialize();
    else accounts = accounts_;
    setRowCount(accounts.size());
    for (int i = 0; i < accounts.size(); i++) {
        const auto& account = accounts[i];
        setItem(i, 0, new QTableWidgetItem(QString::fromStdString(account.user_id())));
        setItem(i, 1, new QTableWidgetItem(QString::fromStdString(account.username())));
        QString priv;
        switch (account.privilege()) {
            case 0:
                priv = "游客";
                break;
            case 1:
                priv = "顾客";
                break;
            case 3:
                priv = "管理员";
                break;
            case 7:
                priv = "店长";
                break;
            default:
                priv = "未知";
                break;
        }
        setItem(i, 2, new QTableWidgetItem(priv));
    }
    resizeRowsToContents();
}

void AccountTable::handleSelectionChanged() {
    auto items = selectedItems();
    if (items.isEmpty()) {
        return;
    }
    int row = items.first()->row();
    QTableWidgetItem* userIdItem = item(row, 0);
    if (!userIdItem) return;
    QString user_id = userIdItem->text();
    emit accountSelected(user_id);
}

void AccountTable::updateAccounts(const std::vector<Account>& accounts) {
    qDebug() << "Updating accounts in AccountTable, new size:" << accounts.size();
    accounts_ = accounts;
    refreshTable();
}

void AccountTable::handleSearch(const std::vector<Account>& accounts) {
    qDebug() << "Handling search in AccountTable, result size:" << accounts.size();
    accounts_ = accounts;
    refreshTable(0);
}