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

void AccountTable::refreshTable() {
    clearContents();
    if (account_manager->current_privilege() < 3) {
        setRowCount(0);
        return;
    }
    const auto& accounts = account_manager->serialize();
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