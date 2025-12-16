#ifndef ACCOUNT_CONTROL_PANEL_HPP
#define ACCOUNT_CONTROL_PANEL_HPP

#include "qt_common.hpp"
#include "globals.hpp"

class AccountControlPanel : public QWidget {
    Q_OBJECT
public:
    AccountControlPanel(QWidget *parent = nullptr);

    void refreshPanel();

    QString selectedUserID() const;

signals:
    void accountListChanged();

    void searchedAccounts(const std::vector<Account>& accounts);

    void loginSucceeded(const QString& user_id);

private slots:
    void onAddButtonClicked();
    void onPasswordButtonClicked();
    void onDeleteButtonClicked();
    void onSearchButtonClicked();
    void onLoginButtonClicked();

public slots:
    void onAccountSelected(const QString& user_id);

private:
    QPushButton *addButton;
    QPushButton *passwordButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;
    QPushButton *loginButton;

    void applyStyle();

    QString currentSelectedUserID;

};

#endif