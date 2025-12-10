#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "qt_common.hpp"
#include "login_dialog.hpp"
#include "register_dialog.hpp"

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

        layout->addWidget(titleLabel, 0, Qt::AlignLeft);
        layout->addStretch(1);
        layout->addWidget(loginButton);
        layout->addWidget(registerButton);

        setMinimumHeight(50);

        connect(loginButton, &QPushButton::clicked, [=]() {
            qDebug() << "登录按钮被点击";
            LoginDialog dialog(this);
            dialog.exec();
        });
        
        connect(registerButton, &QPushButton::clicked, [=]() {
            qDebug() << "注册按钮被点击";
            RegisterDialog dialog(this);
            dialog.exec();
        });
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
        )");
    }

private:
    QLabel *titleLabel;
    QPushButton *loginButton;
    QPushButton *registerButton;
};

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        topBar = new TopBar();
        mainLayout->addWidget(topBar);

        QTextEdit *contentArea = new QTextEdit();
        contentArea->setPlaceholderText("这里是内容区域...");
        contentArea->setStyleSheet("font-size: 14px; padding: 20px;");
        mainLayout->addWidget(contentArea, 1);

        setCentralWidget(centralWidget);
        resize(800, 600);
        setWindowTitle("BookStore");
    }

private:
    TopBar *topBar;
};

#endif