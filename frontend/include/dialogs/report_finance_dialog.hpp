#ifndef REPORT_FINANCE_DIALOG_HPP
#define REPORT_FINANCE_DIALOG_HPP

#include <fstream>
#include <QFileDialog>
#include <QDir>
#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"

class ReportFinanceDialog : public QDialog {
public:
    ReportFinanceDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            const QString dir = QFileDialog::getExistingDirectory(this, "选择导出目录", QDir::homePath());
            if (dir.isEmpty()) {
                return; // user cancelled
            }

            QString fileName = fileNameEdit->text().trimmed();
            if (fileName.isEmpty()) {
                fileName = "FinanceReport.txt";
            }
            if (!fileName.endsWith(".txt", Qt::CaseInsensitive)) {
                fileName += ".txt";
            }

            const QString fullPath = QDir(dir).filePath(fileName);
            std::fstream file(fullPath.toStdString(), std::ios::out);
            if (!file.is_open()) {
                qDebug() << "错误: 无法创建文件!" << fullPath;
                errorLabel->setText("错误: 无法创建文件! " + fullPath);
                errorLabel->show();
                return;
            }

            log_manager->report_finance(file);
            file.close();

            const std::string msg = current_time() + " [REPORT]User " + account_manager->current_user() + " reported finance.";
            log_manager->add_log(msg);

            qDebug() << "财务报表已生成:" << fullPath;
            QMessageBox::information(this, "成功", "财务报表已生成: " + fullPath);
            accept();
        });
    }

private:
    QLineEdit *fileNameEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("生成财务报表");
        setFixedSize(300, 150);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        fileNameEdit = new QLineEdit();
        fileNameEdit->setPlaceholderText("FinanceReport.txt");

        formLayout->addRow("文件名:", fileNameEdit);

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