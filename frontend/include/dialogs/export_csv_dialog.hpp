#ifndef EXPORT_CSV_DIALOG_HPP
#define EXPORT_CSV_DIALOG_HPP

#include <QtCore/qline.h>
#include <fstream>
#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"

class ExportCSVDialog : public QDialog {
public:
    ExportCSVDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString accountFileName = accountFileNameEdit->text().trimmed();
            if (accountFileName.isEmpty()) {
                accountFileName = "Accounts.csv";
            }
            if (!accountFileName.endsWith(".csv")) {
                accountFileName += ".csv";
            }
            try {
                account_manager->export_data(accountFileName.toStdString());
            }
            catch (...) {
                qDebug() << "错误: 无法创建文件!";
                errorLabel->setText("错误: 无法创建文件!");
                errorLabel->show();
                return;
            }
            std::string msg = current_time() + " [EXPORT]User " + account_manager->current_user() + " exported accounts to CSV.";
            log_manager->add_log(msg);
            qDebug() << "账户已导出为CSV:" << accountFileName;
            QMessageBox::information(this, "成功", "账户已导出为CSV: " + accountFileName);

            QString bookFileName = bookFileNameEdit->text().trimmed();
            if (bookFileName.isEmpty()) {
                bookFileName = "Books.csv";
            }
            if (!bookFileName.endsWith(".csv")) {   
                bookFileName += ".csv";
            }
            try {
                book_manager->export_data(bookFileName.toStdString());
            }
            catch (...) {
                qDebug() << "错误: 无法创建文件!";
                errorLabel->setText("错误: 无法创建文件!");
                errorLabel->show();
                return;
            }
            msg = current_time() + " [EXPORT]User " + account_manager->current_user() + " exported books to CSV.";
            log_manager->add_log(msg);
            qDebug() << "图书已导出为CSV:" << bookFileName;
            QMessageBox::information(this, "成功", "图书已导出为CSV:" + bookFileName);
            
            QString logFileName = logFileNameEdit->text().trimmed();
            if (logFileName.isEmpty()) {
                logFileName = "Logs.csv";
            }
            if (!logFileName.endsWith(".csv")) {
                logFileName += ".csv";
            }
            try {
                log_manager->export_data(logFileName.toStdString());
            }
            catch (...) {
                qDebug() << "错误: 无法创建文件!";
                errorLabel->setText("错误: 无法创建文件!");
                errorLabel->show();
                return;
            }
            msg = current_time() + " [EXPORT]User " + account_manager->current_user() + " exported logs to CSV.";
            log_manager->add_log(msg);
            qDebug() << "日志已导出为CSV:" << logFileName;
            QMessageBox::information(this, "成功", "日志已导出为CSV: " + logFileName);
            
            accept();
        });
    }

private:
    QLineEdit *accountFileNameEdit;
    QLineEdit *bookFileNameEdit;
    QLineEdit *logFileNameEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("导出CSV");
        setFixedSize(300, 250);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        accountFileNameEdit = new QLineEdit();
        accountFileNameEdit->setPlaceholderText("Accounts.csv");

        bookFileNameEdit = new QLineEdit();
        bookFileNameEdit->setPlaceholderText("Books.csv");

        logFileNameEdit = new QLineEdit();
        logFileNameEdit->setPlaceholderText("Logs.csv");

        formLayout->addRow("账户文件名:", accountFileNameEdit);
        formLayout->addRow("图书文件名:", bookFileNameEdit);
        formLayout->addRow("日志文件名:", logFileNameEdit);

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