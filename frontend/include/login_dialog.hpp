#ifndef LOGIN_DIALOG_HPP
#define LOGIN_DIALOG_HPP

#include "qt_common.hpp"

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
            accept();
        });
        
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }
    
private:
    QLineEdit *userIDEdit;
    QLineEdit *passwordEdit;
    QDialogButtonBox *buttonBox;

    void setupUI() {
        setWindowTitle("登录");
        setFixedSize(300, 150);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        userIDEdit = new QLineEdit();
        passwordEdit = new QLineEdit();
        passwordEdit->setEchoMode(QLineEdit::Password);
        
        formLayout->addRow("用户ID:", userIDEdit);
        formLayout->addRow("密码:", passwordEdit);
        
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