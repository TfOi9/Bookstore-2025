#ifndef FINANCE_QUERY_DIALOG_HPP
#define FINANCE_QUERY_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"

class FinanceQueryDialog : public QDialog {
public:
    FinanceQueryDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString countStr = countEdit->text();
            int count = -1;
            if (!countStr.isEmpty()) {
                bool ok;
                count = countStr.toInt(&ok);
                if (!ok || count < 0) {
                    qDebug() << "错误: 查询条数格式不正确!";
                    errorLabel->setText("错误: 查询条数格式不正确!");
                    errorLabel->show();
                    return;
                }
            }

            if (log_manager->finance_size() < count) {
                qDebug() << "错误: 查询条数超过现有财务记录数!";
                errorLabel->setText("错误: 查询条数超过现有财务记录数!");
                errorLabel->show();
                return;
            }

            qDebug() << "进行财务查询, 条数: " << (count == -1 ? "全部" : QString::number(count));

            std::string msg = current_time() + " [FIND]User " + account_manager->current_user() + " showed finance.";
            log_manager->add_log(msg);

            auto p = log_manager->finance(count);
            QString result = "收入：" + QString::number(p.first, 'f', 2) + "\n支出：" + QString::number(p.second, 'f', 2);
            QMessageBox::information(this, "查询结果", result);
            
            accept();
        });

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    QLineEdit *countEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("财务查询");
        setFixedSize(300, 150);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        countEdit = new QLineEdit();
        countEdit->setPlaceholderText("留空以查询所有");

        formLayout->addRow("查询条数:", countEdit);

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

            QLabel#priceLabel {
                font-size: 14px;
                color: #004cb6;
                font-weight: bold;
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