#ifndef LOGIN_DIALOG_HPP
#define LOGIN_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "validator.hpp"

class LoginDialog : public QDialog {
public:
    LoginDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString userID = userIDEdit->text();
            QString password = passwordEdit->text();
            
            qDebug() << "登录信息";
            qDebug() << "用户ID:" << userID;
            qDebug() << "密码:" << password;

            if (userID.isEmpty() || password.isEmpty()) {
                qDebug() << "错误: 所有字段均为必填项!";
                errorLabel->setText("错误: 所有字段均为必填项!");
                errorLabel->show();
                return;
            }

            if (!Validator(userID.toStdString()).min_len(1).max_len(30).normal_char_only()) {
                qDebug() << "错误: 用户ID格式不正确!";
                errorLabel->setText("错误: 用户ID格式不正确!");
                errorLabel->show();
                return;
            }

            bool login_success = account_manager->login(userID.toStdString(), password.toStdString());

            if (!login_success) {
                qDebug() << "错误: 登录失败, 用户ID或密码错误!";
                errorLabel->setText("错误: 登录失败, 用户ID或密码错误!");
                errorLabel->show();
                return;
            }

            accept();
        });
        
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }
    
private:
    QLineEdit *userIDEdit;
    QLineEdit *passwordEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("登录");
        setFixedSize(300, 200);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        userIDEdit = new QLineEdit();
        passwordEdit = new QLineEdit();
        passwordEdit->setEchoMode(QLineEdit::Password);
        
        formLayout->addRow("用户ID:", userIDEdit);
        formLayout->addRow("密码:", passwordEdit);

        errorLabel = new QLabel();
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->hide();
        
        buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel
        );

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        
        mainLayout->addLayout(formLayout);
        mainLayout->addWidget(errorLabel);
        mainLayout->addWidget(buttonBox);
    }

    void applyStyle() {
        setStyleSheet(R"(
            QLabel {
                font-size: 14px;
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
};

#endif