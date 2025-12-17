#ifndef REPORT_EMPLOYEE_DIALOG_HPP
#define REPORT_EMPLOYEE_DIALOG_HPP

#include <fstream>
#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"

class ReportEmployeeDialog : public QDialog {
public:
    ReportEmployeeDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString fileName = fileNameEdit->text().trimmed();
            if (fileName.isEmpty()) {
                fileName = "EmployeeReport.txt";
            }
            if (!fileName.endsWith(".txt")) {
                fileName += ".txt";
            }

            std::fstream file(fileName.toStdString(), std::ios::out);
            if (!file.is_open()) {
                qDebug() << "错误: 无法创建文件!";
                errorLabel->setText("错误: 无法创建文件!");
                errorLabel->show();
                return;
            }

            auto vec = account_manager->list_admins();
            for (auto user_id : vec) {
                log_manager->report_employee(user_id, file);
            }
            file.close();

            std::string msg = current_time() + " [REPORT]User " + account_manager->current_user() + " reported employee.";
            log_manager->add_log(msg);

            qDebug() << "员工报表已生成:" << fileName;
            QMessageBox::information(this, "成功", "员工报表已生成: " + fileName);
            accept();
        });
    }

private:
    QLineEdit *fileNameEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("生成员工报表");
        setFixedSize(300, 150);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        fileNameEdit = new QLineEdit();
        fileNameEdit->setPlaceholderText("EmployeeReport.txt");

        formLayout->addRow("文件名:", fileNameEdit);

        errorLabel = new QLabel();
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->hide();

        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

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