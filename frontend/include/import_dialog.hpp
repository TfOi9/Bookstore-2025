#ifndef IMPORT_DIALOG_HPP
#define IMPORT_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"

class ImportDialog : public QDialog {
public:
    ImportDialog(const QString& ISBN, QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        Book book = book_manager->find_ISBN(string_to_array<20>(ISBN.toStdString()))[0];
        ISBNLabel->setText(ISBN);
        nameLabel->setText(QString::fromStdString(book.book_name()));
        quantityEdit->setValue(1);
        connect(buttonBox, &QDialogButtonBox::accepted, [this, ISBN]() {
            int quantity = quantityEdit->value();
            if (quantity <= 0) {
                qDebug() << "错误: 进货数量必须大于0!";
                errorLabel->setText("错误: 进货数量必须大于0!");
                errorLabel->show();
                return;
            }

            QString costStr = costEdit->text();
            if (!Validator(costStr.toStdString()).max_len(13).number_and_dot_only().only_one_dot()) {
                qDebug() << "错误: 总价格式不正确!";
                errorLabel->setText("错误: 总价格式不正确!");
                errorLabel->show();
                return;
            }


            qDebug() << "进货总价字符串:" << costStr;
            double cost = 0.00;
            try {
                cost = std::stod(costStr.toStdString());
            }
            catch (...) {
                qDebug() << "错误: 总价格式不正确!";
                errorLabel->setText("错误: 总价格式不正确!");
                errorLabel->show();
                return;
            }

            if (cost <= 0.00) {
                qDebug() << "错误: 总价格必须大于0!";
                errorLabel->setText("错误: 总价格必须大于0!");
                errorLabel->show();
                return;
            }

            if (cost > 2147483647.99) {
                qDebug() << "错误: 总价格超过上限!";
                errorLabel->setText("错误: 总价格超过上限!");
                errorLabel->show();
                return;
            }

            book_manager->import(string_to_array<20>(ISBN.toStdString()), quantity);

            std::string msg = current_time() + " [SELECT]User " + account_manager->current_user() + " selected book " + ISBN.toStdString() + '.';
            log_manager->add_log(msg);

            msg = current_time() + " [IMPORT]User " + account_manager->current_user() + " imported book " + ISBN.toStdString() + ' ' + quantityEdit->text().toStdString() + " copies, costing " + costStr.toStdString() + '.';
            log_manager->add_log(msg);
            log_manager->add_employee_log(account_manager->current_user(), msg);
            log_manager->add_finance_log(-cost);

            qDebug() << "进货成功! 总价:" << QString::number(cost, 'f', 2);
            QMessageBox::information(this, "进货成功", "进货成功! 总价: " + QString::number(cost, 'f', 2));

            accept();
        });
    }

private:
    QLabel *ISBNLabel;
    QLabel *nameLabel;
    QSpinBox *quantityEdit;
    QLineEdit *costEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;
    
    void setupUI() {
        setWindowTitle("进货");
        setFixedSize(300, 250);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        ISBNLabel = new QLabel();
        nameLabel = new QLabel();
        quantityEdit = new QSpinBox();
        quantityEdit->setMinimum(1);
        quantityEdit->setValue(1);
        costEdit = new QLineEdit();
        
        formLayout->addRow("ISBN:", ISBNLabel);
        formLayout->addRow("书名:", nameLabel);
        formLayout->addRow("数量:", quantityEdit);
        formLayout->addRow("总价:", costEdit);

        errorLabel = new QLabel();
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->hide();

        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(quantityEdit, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int){});

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