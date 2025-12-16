#ifndef MODIFY_PASSWORD_DIALOG_HPP
#define MODIFY_PASSWORD_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"

class ModifyPasswordDialog : public QDialog {
public:
    ModifyPasswordDialog(const QString& user_id, QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        userIDEdit->setText(user_id);
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString userID = userIDEdit->text();
            QString oldPassword = oldPasswordEdit->text();
            QString newPassword = newPasswordEdit->text();
            QString newPasswordConfirm = newPasswordConfirmEdit->text();

            qDebug() << "修改密码信息";
            qDebug() << "旧密码:" << oldPassword;
            qDebug() << "新密码:" << newPassword;

            if (userID.isEmpty() || newPassword.isEmpty()) {
                qDebug() << "错误: 用户ID和新密码为必填项!";
                errorLabel->setText("错误: 用户ID和新密码为必填项!");
                errorLabel->show();
                return;
            }

            if (account_manager->count_userID(userID.toStdString()) == 0) {
                qDebug() << "错误: 该用户不存在!";
                errorLabel->setText("错误: 该用户不存在!");
                errorLabel->show();
                return;
            }

            if (!Validator(newPassword.toStdString()).min_len(4).max_len(30).normal_char_only()) {
                qDebug() << "错误: 新密码格式不正确!";
                errorLabel->setText("错误: 新密码格式不正确!");
                errorLabel->show();
                return;
            }

            if (newPassword != newPasswordConfirm) {
                qDebug() << "错误: 两次输入的新密码不匹配!";
                errorLabel->setText("错误: 两次输入的新密码不匹配!");
                errorLabel->show();
                return;
            }

            bool success = account_manager->change_password(userID.toStdString(), newPassword.toStdString(), oldPassword.toStdString());
            if (!success) {
                qDebug() << "错误: 修改密码失败!";
                errorLabel->setText("错误: 修改密码失败!");
                errorLabel->show();
                return;
            }

            qDebug() << "密码修改成功!";

            std::string msg = current_time() + " [PASSWORD]User " + userID.toStdString() + " changed password.";
            log_manager->add_log(msg);

            accept();
        });

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    QLineEdit *userIDEdit;
    QLineEdit *oldPasswordEdit;
    QLineEdit *newPasswordEdit;
    QLineEdit *newPasswordConfirmEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("修改密码");
        setFixedSize(400, 300);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        userIDEdit = new QLineEdit(this);
        formLayout->addRow("用户ID:", userIDEdit);

        oldPasswordEdit = new QLineEdit(this);
        oldPasswordEdit->setEchoMode(QLineEdit::Password);
        oldPasswordEdit->setPlaceholderText("高权限可免填");
        formLayout->addRow("旧密码:", oldPasswordEdit);

        newPasswordEdit = new QLineEdit(this);
        newPasswordEdit->setEchoMode(QLineEdit::Password);
        formLayout->addRow("新密码:", newPasswordEdit);

        newPasswordConfirmEdit = new QLineEdit(this);
        newPasswordConfirmEdit->setEchoMode(QLineEdit::Password);
        formLayout->addRow("确认新密码:", newPasswordConfirmEdit);

        errorLabel = new QLabel(this);
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->hide();
        
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

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