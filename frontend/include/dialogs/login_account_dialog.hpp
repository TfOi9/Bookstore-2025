#ifndef LOGIN_ACCOUNT_DIALOG_HPP
#define LOGIN_ACCOUNT_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "validator.hpp"

class LoginAccountDialog : public QDialog {
public:
    LoginAccountDialog(const QString &userID, QWidget *parent = nullptr) : QDialog(parent), userID_(userID) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString password = passwordEdit->text();
            errorLabel->hide();
            userID_ = userIDEdit->text();

            if (userIDEdit->text().isEmpty()) {
                errorLabel->setText("错误: 未选择账户!");
                errorLabel->show();
                return;
            }

            if (account_manager->count_userID(userID_.toStdString()) == 0) {
                errorLabel->setText("错误: 该用户不存在!");
                errorLabel->show();
                return;
            }

            if (password.isEmpty() && account_manager->current_privilege() < 7) {
                errorLabel->setText("错误: 密码不能为空!");
                errorLabel->show();
                return;
            }

            if (account_manager->current_privilege() == 7) {
                password = QString::fromStdString(account_manager->find(userID_.toStdString())[0].password());
            }

            account_manager->logout();
            bool login_success = account_manager->login(userID_.toStdString(), password.toStdString());
            if (!login_success) {
                errorLabel->setText("错误: 登录失败, 密码错误或账户不存在!");
                errorLabel->show();
                return;
            }

            std::string msg = current_time() + " [LOGIN]User " + userID_.toStdString() + " logged in.";
            log_manager->add_log(msg);
            accept();
        });

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    QString userID_;
    QLineEdit *userIDEdit;
    QLineEdit *passwordEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("登录账户");
        setFixedSize(300, 200);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        userIDEdit = new QLineEdit();
        userIDEdit->setText(userID_);

        passwordEdit = new QLineEdit();
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordEdit->setPlaceholderText("高权限可免填");

        formLayout->addRow("用户ID:", userIDEdit);
        formLayout->addRow("密码:", passwordEdit);

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
