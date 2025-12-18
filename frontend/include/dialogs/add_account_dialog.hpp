#ifndef ADD_ACCOUNT_DIALOG_HPP
#define ADD_ACCOUNT_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"

class AddAccountDialog : public QDialog {
public:
    AddAccountDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString userIDStr = userIDEdit->text();
            QString username = usernameEdit->text();
            QString password = passwordEdit->text();

            qDebug() << "添加账户信息";
            qDebug() << "用户ID:" << userIDStr;
            qDebug() << "用户名:" << username;
            qDebug() << "密码:" << password;

            if (userIDStr.isEmpty() || username.isEmpty() || password.isEmpty()) {
                qDebug() << "错误: 所有字段均为必填项!";
                errorLabel->setText("错误: 所有字段均为必填项!");
                errorLabel->show();
                return;
            }

            if (!Validator(userIDStr.toStdString()).max_len(30).normal_char_only()) {
                qDebug() << "错误: 用户ID格式不正确!";
                errorLabel->setText("错误: 用户ID格式不正确!");
                errorLabel->show();
                return;
            }

            if (account_manager->count_userID(userIDStr.toStdString())) {
                qDebug() << "错误: 用户ID已存在!";
                errorLabel->setText("错误: 用户ID已存在!");
                errorLabel->show();
                return;
            }

            if (!UnicodeValidator(username.toStdString()).max_len(30).han()) {
                qDebug() << "错误: 用户名格式不正确!";
                errorLabel->setText("错误: 用户名格式不正确!");
                errorLabel->show();
                return;
            }

            if (!Validator(password.toStdString()).min_len(4).max_len(30).normal_char_only()) {
                qDebug() << "错误: 密码格式不正确!";
                errorLabel->setText("错误: 密码格式不正确!");
                errorLabel->show();
                return;
            }

            if (password != passwordConfirmEdit->text()) {
                qDebug() << "错误: 两次输入的密码不匹配!";
                errorLabel->setText("错误: 两次输入的密码不匹配!");
                errorLabel->show();
                return;
            }

            int privilege = privilegeComboBox->currentText() == "管理员" ? 3 : 1;
            account_manager->add_account(userIDStr.toStdString(), username.toStdString(), password.toStdString(), privilege);
            qDebug() << "账户添加成功!";

            std::string msg = current_time() + " [USERADD]Added user " + userIDStr.toStdString() + '.';
            log_manager->add_log(msg);

            accept();
        });
    }

private:
    QLineEdit *userIDEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *passwordConfirmEdit;
    QComboBox *privilegeComboBox;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("添加账户");
        setFixedSize(400, 300);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        userIDEdit = new QLineEdit();
        usernameEdit = new QLineEdit();
        passwordEdit = new QLineEdit();
        passwordConfirmEdit = new QLineEdit();
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordConfirmEdit->setEchoMode(QLineEdit::Password);
        privilegeComboBox = new QComboBox();
        privilegeComboBox->addItem("顾客", 1);
        if (account_manager->current_privilege() > 3) privilegeComboBox->addItem("管理员", 3);
        
        formLayout->addRow("用户ID:", userIDEdit);
        formLayout->addRow("用户名:", usernameEdit);
        formLayout->addRow("密码:", passwordEdit);
        formLayout->addRow("确认密码:", passwordConfirmEdit);
        formLayout->addRow("权限等级:", privilegeComboBox);

        errorLabel = new QLabel();
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->hide();

        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

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