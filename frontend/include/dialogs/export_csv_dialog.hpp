#ifndef EXPORT_CSV_DIALOG_HPP
#define EXPORT_CSV_DIALOG_HPP

#include <QtCore/qline.h>
#include <QFileDialog>
#include <functional>
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
            const QString baseDir = QFileDialog::getExistingDirectory(this, "选择导出目录", QDir::homePath());
            if (baseDir.isEmpty()) {
                return;
            }

            auto buildPath = [](const QString &dir, const QString &name, const QString &fallback) {
                QString fname = name.trimmed();
                if (fname.isEmpty()) {
                    fname = fallback;
                }
                if (!fname.endsWith(".csv", Qt::CaseInsensitive)) {
                    fname += ".csv";
                }
                return QDir(dir).filePath(fname);
            };

            const QString accountPath = buildPath(baseDir, accountFileNameEdit->text(), "Accounts.csv");
            const QString bookPath    = buildPath(baseDir, bookFileNameEdit->text(),    "Books.csv");
            const QString logPath     = buildPath(baseDir, logFileNameEdit->text(),     "Logs.csv");

            if (!exportWithMessage(accountPath, [this, accountPath]() { account_manager->export_data(accountPath.toStdString()); }, "账户")) return;
            if (!exportWithMessage(bookPath,    [this, bookPath]()    { book_manager->export_data(bookPath.toStdString()); },       "图书")) return;
            if (!exportWithMessage(logPath,     [this, logPath]()     { log_manager->export_data(logPath.toStdString()); },        "日志")) return;

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

    bool exportWithMessage(const QString &path, const std::function<void()> &fn, const QString &label) {
        try {
            fn();
        } catch (...) {
            qDebug() << "错误: 无法创建文件!" << path;
            errorLabel->setText("错误: 无法创建文件! " + path);
            errorLabel->show();
            return false;
        }
        const std::string msg = current_time() + " [EXPORT]User " + account_manager->current_user() + " exported " + label.toStdString() + " to CSV.";
        log_manager->add_log(msg);
        qDebug() << label << "已导出为CSV:" << path;
        QMessageBox::information(this, "成功", label + "已导出为CSV: " + path);
        return true;
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