#ifndef SEARCH_ACCOUNT_DIALOG_HPP
#define SEARCH_ACCOUNT_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"
#include "account_control_panel.hpp"

class SearchAccountDialog : public QDialog {
public:
    SearchAccountDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this, p = qobject_cast<AccountControlPanel*>(parent)]() {
            QString searchType = comboBox->currentText();
            QString searchContent = searchEdit->text();
            if (searchType == "权限等级") {
                searchContent = privilegeCombo->currentData().toString();
            }
            qDebug() << "搜索账户信息";
            qDebug() << "搜索类型:" << searchType;
            qDebug() << "搜索内容:" << searchContent;

            if (searchType != "全部账户" && searchContent.isEmpty()) {
                qDebug() << "错误: 搜索内容不能为空!";
                errorLabel->setText("错误: 搜索内容不能为空!");
                errorLabel->show();
                return;
            }

            if (searchType == "全部账户") {
                if (p) {
                    std::string msg = current_time() + " [FIND]User " + account_manager->current_user() + " found all accounts.";
                    log_manager->add_log(msg);
                    emit p->searchedAccounts(account_manager->serialize());
                }
            }
            else if (searchType == "用户ID") {
                if (!Validator(searchContent.toStdString()).max_len(30).normal_char_only()) {
                    qDebug() << "错误: 用户ID格式不正确!";
                    errorLabel->setText("错误: 用户ID格式不正确!");
                    errorLabel->show();
                    return;
                }
                if (p) {
                    std::string msg = current_time() + " [FIND]User " + account_manager->current_user() + " found accounts bu user ID.";
                    log_manager->add_log(msg);
                    auto accounts = account_manager->find(searchContent.toStdString());
                    emit p->searchedAccounts(accounts);
                }
            }
            else if (searchType == "用户名") {
                if (!Validator(searchContent.toStdString()).max_len(30).visible_only()) {
                    qDebug() << "错误: 用户名格式不正确!";
                    errorLabel->setText("错误: 用户名格式不正确!");
                    errorLabel->show();
                    return;
                }
                if (p) {
                    std::string msg = current_time() + " [FIND]User " + account_manager->current_user() + " found accounts by username.";
                    log_manager->add_log(msg);
                    auto accounts = account_manager->find_username(searchContent.toStdString());
                    emit p->searchedAccounts(accounts);
                }
            }
            else if (searchType == "权限等级") {
                bool ok;
                int privilege = searchContent.toInt(&ok);
                if (!ok || privilege != 1 && privilege != 3 && privilege != 7) {
                    qDebug() << "错误: 权限等级格式不正确!";
                    errorLabel->setText("错误: 权限等级格式不正确!");
                    errorLabel->show();
                    return;
                }
                if (p) {
                    std::string msg = current_time() + " [FIND]User " + account_manager->current_user() + " found accounts by privilege.";
                    log_manager->add_log(msg);
                    auto accounts = account_manager->find_privilege(privilege);
                    emit p->searchedAccounts(accounts);
                }
            }

            accept();
        });

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    QComboBox *comboBox;
    QComboBox *privilegeCombo;
    QLineEdit *searchEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("搜索账户");
        setFixedSize(300, 250);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        comboBox = new QComboBox();
        comboBox->addItems({"全部账户","用户ID", "用户名","权限等级"});

        searchEdit = new QLineEdit();
        searchEdit->setEnabled(false);

        privilegeCombo = new QComboBox();
        privilegeCombo->addItem("顾客", 1);
        privilegeCombo->addItem("管理员", 3);
        privilegeCombo->addItem("店长", 7);
        privilegeCombo->setVisible(false);
        privilegeCombo->setEnabled(false);

        connect(comboBox, &QComboBox::currentTextChanged, this, [this](const QString &text){
            if (text == "全部账户") {
                searchEdit->setEnabled(false);
                searchEdit->setVisible(true);
                privilegeCombo->setVisible(false);
                privilegeCombo->setEnabled(false);
            }
            else if (text == "权限等级") {
                searchEdit->setVisible(false);
                searchEdit->setEnabled(false);
                privilegeCombo->setVisible(true);
                privilegeCombo->setEnabled(true);
            }
            else {
                searchEdit->setEnabled(true);
                searchEdit->setVisible(true);
                privilegeCombo->setVisible(false);
                privilegeCombo->setEnabled(false);
            }
        });
        
        formLayout->addRow("搜索类型:", comboBox);
        formLayout->addRow("搜索内容:", searchEdit);
        formLayout->addRow("权限选择:", privilegeCombo);

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