#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "login_dialog.hpp"
#include "register_dialog.hpp"
#include "profile_dialog.hpp"
#include <QStatusBar>
#include <functional>

class TopBar : public QWidget {
public:
    TopBar(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
        applyStyle();
    }

private:
    void setupUI() {
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setContentsMargins(20, 10, 20, 10);
        layout->setSpacing(0);

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

        connect(loginButton, &QPushButton::clicked, [=]() {
            qDebug() << "登录按钮被点击";
            LoginDialog dialog(this);
            dialog.exec();
            refreshAuthUI();
        });
        
        connect(registerButton, &QPushButton::clicked, [=]() {
            qDebug() << "注册按钮被点击";
            RegisterDialog dialog(this);
            dialog.exec();
            refreshAuthUI();
        });

        QMenu *userMenu = new QMenu(userButton);
        QAction *profileAction = userMenu->addAction("个人信息");
        QAction *logoutAction = userMenu->addAction("注销");

        connect(profileAction, &QAction::triggered, [=]() {
            qDebug() << "个人信息按钮被点击";
            ProfileDialog dialog(this);
            dialog.exec();
            refreshAuthUI();
        });

        connect(logoutAction, &QAction::triggered, [=]() {
            bool ok = account_manager->logout();
            if (ok) {
                QMessageBox::information(this, "注销", "已成功注销");
            } else {
                QMessageBox::warning(this, "注销失败", "当前没有登录的用户");
            }
            refreshAuthUI();
        });

        userButton->setMenu(userMenu);

        refreshAuthUI();
    }

    void applyStyle() {
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

    void refreshAuthUI() {
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
        }
        else if (pri == 1) {
            accountButton->hide();
            bookButton->setText("书籍浏览");
            bookButton->show();
            logButton->hide();
        }
        else {
            accountButton->hide();
            bookButton->hide();
            logButton->hide();
        }
        
        QString status;
        if (cur.empty()) {
            status = QStringLiteral("游客模式");
        } else {
            if (pri == 7) status = QStringLiteral("店长模式");
            else if (pri == 3) status = QStringLiteral("管理员模式");
            else if (pri == 1) status = QStringLiteral("顾客模式");
            else status = QStringLiteral("游客模式");
        }
        if (authCallback) authCallback(status);
    }

public:
    void setAuthChangedCallback(std::function<void(const QString &)> cb) {
        authCallback = std::move(cb);
    }

private:
    QLabel *titleLabel;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *accountButton;
    QPushButton *bookButton;
    QPushButton *logButton;
    QPushButton *userButton;
    std::function<void(const QString &)> authCallback;
};

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        topBar = new TopBar(this);
        mainLayout->addWidget(topBar);

        topBar->setAuthChangedCallback([this](const QString &s){ setStatusMessage(s); });

        QTextEdit *contentArea = new QTextEdit();
        contentArea->setPlaceholderText("这里是内容区域...");
        contentArea->setStyleSheet("font-size: 14px; padding: 20px;");
        mainLayout->addWidget(contentArea, 1);

        setCentralWidget(centralWidget);
        
        statusBar()->showMessage(QStringLiteral("游客模式"));
        resize(800, 600);
        setWindowTitle("BookStore");
    }

    void setStatusMessage(const QString &msg, int timeout = 0) {
        statusBar()->showMessage(msg, timeout);
    }

private:
    TopBar *topBar;
};

#endif