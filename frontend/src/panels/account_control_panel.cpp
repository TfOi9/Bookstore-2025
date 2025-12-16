#include "account_control_panel.hpp"
#include "account.hpp"
#include "add_account_dialog.hpp"
#include "modify_password_dialog.hpp"
#include "delete_account_dialog.hpp"
#include "globals.hpp"
#include "login_account_dialog.hpp"

AccountControlPanel::AccountControlPanel(QWidget *parent) : QWidget(parent) {
    addButton = new QPushButton("添加", this);
    passwordButton = new QPushButton("修改密码", this);
    deleteButton = new QPushButton("删除", this);
    searchButton = new QPushButton("搜索", this);
    loginButton = new QPushButton("登录", this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch(1);
    layout->addWidget(addButton);
    layout->addWidget(passwordButton);
    layout->addWidget(deleteButton);
    layout->addWidget(loginButton);
    layout->addWidget(searchButton);
    
    setLayout(layout);

    applyStyle();

    connect(addButton, &QPushButton::clicked, this, &AccountControlPanel::onAddButtonClicked);
    connect(passwordButton, &QPushButton::clicked, this, &AccountControlPanel::onPasswordButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &AccountControlPanel::onDeleteButtonClicked);
    connect(searchButton, &QPushButton::clicked, this, &AccountControlPanel::onSearchButtonClicked);
    connect(loginButton, &QPushButton::clicked, this, &AccountControlPanel::onLoginButtonClicked);
    
    refreshPanel();
}

void AccountControlPanel::onAccountSelected(const QString& user_id) {
    qDebug() << "Account selected:" << user_id;
    currentSelectedUserID = user_id;
}

void AccountControlPanel::onAddButtonClicked() {
    qDebug() << "Add account button clicked";
    AddAccountDialog dialog(this);
    dialog.exec();
    emit accountListChanged();
}

void AccountControlPanel::onPasswordButtonClicked() {
    qDebug() << "Change password button clicked";
    ModifyPasswordDialog dialog(currentSelectedUserID, this);
    dialog.exec();
}

void AccountControlPanel::onDeleteButtonClicked() {
    qDebug() << "Delete account button clicked";
    DeleteAccountDialog dialog(currentSelectedUserID, this);
    dialog.exec();
    emit accountListChanged();
}

void AccountControlPanel::onSearchButtonClicked() {
    qDebug() << "Search account button clicked";
}

void AccountControlPanel::onLoginButtonClicked() {
    qDebug() << "Login button clicked";
    LoginAccountDialog dialog(currentSelectedUserID, this);
    if (dialog.exec() == QDialog::Accepted) {
        emit loginSucceeded(currentSelectedUserID);
    }
}

void AccountControlPanel::refreshPanel() {
    int pri = account_manager->current_privilege();
    if (pri >= 7) {
        addButton->show();
        passwordButton->show();
        deleteButton->show();
        searchButton->show();
        loginButton->show();
    }
    else if (pri == 3) {
        addButton->hide();
        passwordButton->show();
        deleteButton->hide();
        searchButton->show();
        loginButton->show();
    }
    else {
        addButton->hide();
        passwordButton->hide();
        deleteButton->hide();
        searchButton->hide();
        loginButton->hide();
    }
}

void AccountControlPanel::applyStyle() {
    setStyleSheet(R"(        
        QPushButton {
            background-color: transparent;
            border: none;
            color: #666666;
            font-size: 14px;
            padding: 8px 16px;
            border-radius: 4px;
        }
        
        QPushButton:hover {
            background-color: #f5f5f5;
            color: #333333;
        }
    )");
}

QString AccountControlPanel::selectedUserID() const {
    return currentSelectedUserID;
}