#include "top_bar.hpp"
#include "globals.hpp"
#include "login_dialog.hpp"
#include "register_dialog.hpp"
#include "profile_dialog.hpp"

TopBar::TopBar(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(5);

    titleLabel = new QLabel("BookStore", this);
    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);
    accountButton = new QPushButton("账户管理", this);
    bookButton = new QPushButton("图书管理", this);
    logButton = new QPushButton("日志管理", this);

    userButton = new QPushButton(this);
    userButton->setObjectName("userButton");
    userButton->setFlat(true);
    userButton->setCursor(Qt::PointingHandCursor);

    layout->addWidget(titleLabel, 0, Qt::AlignLeft);
    layout->addWidget(accountButton);
    layout->addWidget(bookButton);
    layout->addWidget(logButton);
    layout->addStretch(1);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    layout->addWidget(userButton);

    setMinimumHeight(50);

    connect(loginButton, &QPushButton::clicked, this, &TopBar::onLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &TopBar::onRegisterButtonClicked);
    connect(accountButton, &QPushButton::clicked, this, &TopBar::onAccountButtonClicked);
    connect(bookButton, &QPushButton::clicked, this, &TopBar::onBookButtonClicked);
    connect(logButton, &QPushButton::clicked, this, &TopBar::onLogButtonClicked);

    QMenu *userMenu = new QMenu(userButton);
    QAction *profileAction = userMenu->addAction("个人信息");
    QAction *logoutAction = userMenu->addAction("注销");

    connect(profileAction, &QAction::triggered, this, &TopBar::onProfileButtonClicked);
    connect(logoutAction, &QAction::triggered, this, &TopBar::onLogoutButtonClicked);

    userButton->setMenu(userMenu);

    applyStyle();
}

void TopBar::onLoginButtonClicked() {
    qDebug() << "登录按钮被点击";
    LoginDialog dialog(this);
    dialog.exec();
    refreshBar();
}

void TopBar::onRegisterButtonClicked() {
    qDebug() << "注册按钮被点击";
    RegisterDialog dialog(this);
    dialog.exec();
    refreshBar();
}

void TopBar::onLogoutButtonClicked() {
    qDebug() << "注销按钮被点击";
    bool ok = account_manager->logout();
    if (ok) {
        QMessageBox::information(this, "注销", "已成功注销");
    } else {
        QMessageBox::warning(this, "注销失败", "当前没有登录的用户");
    }
    refreshBar();
}

void TopBar::onProfileButtonClicked() {
    qDebug() << "个人信息按钮被点击";
    ProfileDialog dialog(this);
    dialog.exec();
    refreshBar();
}

void TopBar::onAccountButtonClicked() {
    qDebug() << "账户管理按钮被点击";
}

void TopBar::onBookButtonClicked() {
    qDebug() << "图书管理按钮被点击";
    emit bookButtonClicked();
}

void TopBar::onLogButtonClicked() {
    qDebug() << "日志管理按钮被点击";
}

void TopBar::refreshBar() {
    std::string cur = account_manager->current_user();
    if (!cur.empty()) {
        userButton->setText(QString::fromStdString(cur));
        userButton->show();
        loginButton->hide();
        registerButton->hide();
    }
    else {
        userButton->hide();
        loginButton->show();
        registerButton->show();
    }
    int pri = account_manager->current_privilege();
    if (pri >= 3) {
        accountButton->show();
        bookButton->setText("书籍管理");
        bookButton->show();
        logButton->show();
        userButton->show();
    }
    else if (pri == 1) {
        accountButton->hide();
        bookButton->setText("书籍浏览");
        bookButton->show();
        logButton->hide();
        userButton->show();
    }
    else {
        accountButton->hide();
        bookButton->hide();
        logButton->hide();
        userButton->hide();
    }

    QString msg;
    switch (account_manager->current_privilege()) {
        case 0:
            msg = QStringLiteral("游客模式");
            break;
        case 1:
            msg = QStringLiteral("顾客模式");
            break;
        case 3:
            msg = QStringLiteral("管理员模式");
            break;
        case 7:
            msg = QStringLiteral("店长模式");
            break;
        default:
            msg = QStringLiteral("");
            break;
    }
    emit authChanged(msg);
    if (authChangedCallback) {
        authChangedCallback(msg);
    }
}

void TopBar::applyStyle() {
    setStyleSheet(R"(
        TopBar {
            background-color: #ffffff;
            border-bottom: 1px solid #e0e0e0;
        }
        
        QLabel {
            font-size: 20px;
            font-weight: bold;
            color: #333333;
        }
        
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

        QPushButton#userButton {
            background-color: transparent;
            border: none;
            color: #666666;
            font-size: 14px;
            padding: 8px 16px;
            border-radius: 4px;
        }

        QPushButton#userButton:hover {
            background-color: #f5f5f5;
            color: #333333;
        }

        QMenu {
            background-color: #ffffff;
            border: 1px solid #e0e0e0;
            padding: 4px 0;
        }

        QMenu::item {
            padding: 8px 16px;
            color: #333333;
        }

        QMenu::item:selected {
            background-color: #f5f5f5;
        }
    )");
}