#ifndef PROFILE_DIALOG_HPP
#define PROFILE_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "validator.hpp"

class ProfileDialog : public QDialog {
public:
    ProfileDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString oldPassword = passwordOldEdit->text();
            QString newPassword = passwordEdit->text();
            QString confirmPassword = passwordConfirmEdit->text();

            if (newPassword.isEmpty() && confirmPassword.isEmpty()) {
                accept();
                return;
            }
            
            qDebug() << "修改密码";

            if (!Validator(newPassword.toStdString()).min_len(4).max_len(30).normal_char_only()) {
                qDebug() << "错误: 新密码格式不正确!";
                errorLabel->setText("错误: 新密码格式不正确!");
                errorLabel->show();
                return;
            }

            if (newPassword != confirmPassword) {
                qDebug() << "错误: 密码和确认密码不匹配!";
                errorLabel->setText("错误: 密码和确认密码不匹配!");
                errorLabel->show();
                return;
            }

            bool change_success = account_manager->change_password(
                account_manager->current_user(),
                newPassword.toStdString(),
                oldPassword.toStdString()
            );

            if (!change_success) {
                qDebug() << "错误: 密码修改失败!";
                errorLabel->setText("错误: 密码修改失败!");
                errorLabel->show();
                return;
            }
            
            qDebug() << "密码修改成功!";

            std::string msg = current_time() + " [PASSWORD]User " + account_manager->current_user() + " changed password.";
            log_manager->add_log(msg);

            accept();
        });
        
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }
private:
    QLabel *userIDLabel;
    QLabel *usernameLabel;
    QLineEdit *passwordOldEdit;
    QLineEdit *passwordEdit;
    QLineEdit *passwordConfirmEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("个人信息");
        setFixedSize(300, 250);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        userIDLabel = new QLabel();
        userIDLabel->setText(QString::fromStdString(account_manager->current_user()));
        usernameLabel = new QLabel();
        usernameLabel->setText(QString::fromStdString(account_manager->current_username()));

        passwordOldEdit = new QLineEdit();
        passwordOldEdit->setEchoMode(QLineEdit::Password);
        passwordEdit = new QLineEdit();
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordConfirmEdit = new QLineEdit();
        passwordConfirmEdit->setEchoMode(QLineEdit::Password);

        formLayout->addRow("用户ID:", userIDLabel);
        formLayout->addRow("用户名:", usernameLabel);
        formLayout->addRow("密码:", passwordOldEdit);
        formLayout->addRow("修改密码:", passwordEdit);
        formLayout->addRow("确认密码:", passwordConfirmEdit);

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