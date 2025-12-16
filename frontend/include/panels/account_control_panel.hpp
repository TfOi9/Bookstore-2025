#ifndef ACCOUNT_CONTROL_PANEL_HPP
#define ACCOUNT_CONTROL_PANEL_HPP

#include "qt_common.hpp"
#include "globals.hpp"

class AccountControlPanel : public QWidget {
    Q_OBJECT
public:
    AccountControlPanel(QWidget *parent = nullptr);

    void refreshPanel();

signals:
    void accountListChanged();

private slots:
    void onAddButtonClicked();
    void onPasswordButtonClicked();
    void onDeleteButtonClicked();
    void onSearchButtonClicked();
    void onLoginButtonClicked();

private:
    QPushButton *addButton;
    QPushButton *passwordButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;
    QPushButton *loginButton;

    void applyStyle();

};

#endif