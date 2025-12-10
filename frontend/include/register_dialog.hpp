#ifndef REGISTER_DIALOG_HPP
#define REGISTER_DIALOG_HPP

#include "qt_common.hpp"

class RegisterDialog : public QDialog {
public:
    RegisterDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString userID = userIDEdit->text();
            QString username = usernameEdit->text();
            QString password = passwordEdit->text();
            QString passwordConfirm = passwordConfirmEdit->text();
            
            qDebug() << "注册信息";
            qDebug() << "用户ID:" << userID;
            qDebug() << "用户名:" << username;
            qDebug() << "密码:" << password;
            qDebug() << "确认密码:" << passwordConfirm;

            if (password != passwordConfirm) {
                qDebug() << "错误: 密码和确认密码不匹配!";
                return;
            }

            accept();
        });
        
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }
    
private:
    QLineEdit *userIDEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *passwordConfirmEdit;
    QDialogButtonBox *buttonBox;

    void setupUI() {
        setWindowTitle("注册");
        setFixedSize(300, 200);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        userIDEdit = new QLineEdit();
        usernameEdit = new QLineEdit();
        passwordEdit = new QLineEdit();
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordConfirmEdit = new QLineEdit();
        passwordConfirmEdit->setEchoMode(QLineEdit::Password);
        
        formLayout->addRow("用户ID:", userIDEdit);
        formLayout->addRow("用户名:", usernameEdit);
        formLayout->addRow("密码:", passwordEdit);
        formLayout->addRow("确认密码:", passwordConfirmEdit);
        
        buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel
        );

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        
        mainLayout->addLayout(formLayout);
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