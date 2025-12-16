#ifndef DELETE_ACCOUNT_DIALOG_HPP
#define DELETE_ACCOUNT_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"

class DeleteAccountDialog : public QDialog {
public:
    DeleteAccountDialog(const QString& user_id, QWidget* parent = nullptr) {
        setupUI();
        applyStyle();
        userIDEdit->setText(user_id);
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString userID = userIDEdit->text();

            qDebug() << "删除账户信息";
            qDebug() << "用户ID:" << userID;

            if (userID.isEmpty()) {
                qDebug() << "错误: 用户ID为必填项!";
                errorLabel->setText("错误: 用户ID为必填项!");
                errorLabel->show();
                return;
            }

            if (account_manager->count_userID(userID.toStdString()) == 0) {
                qDebug() << "错误: 该用户不存在!";
                errorLabel->setText("错误: 该用户不存在!");
                errorLabel->show();
                return;
            }

            bool success = account_manager->delete_account(userID.toStdString());
            if (!success) {
                qDebug() << "错误: 删除账户失败!";
                errorLabel->setText("错误: 删除账户失败!");
                errorLabel->show();
                return;
            }

            std::string msg = current_time() + " [USERDEL]Deleted user " + userID.toStdString() + '.';
            log_manager->add_log(msg);

            accept();
        });
    }

private:
    QLineEdit *userIDEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("删除账户");
        setFixedSize(300, 150);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        userIDEdit = new QLineEdit(this);
        formLayout->addRow("用户ID:", userIDEdit);

        errorLabel = new QLabel(this);
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->setText("警告：删除账户后不可恢复！");

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