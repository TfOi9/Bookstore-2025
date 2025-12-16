#ifndef BUY_DIALOG_HPP
#define BUY_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"

class BuyDialog : public QDialog {
public:
    BuyDialog(const QString& ISBN, QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        Book book = book_manager->find_ISBN(string_to_array<20>(ISBN.toStdString()))[0];
        ISBNLabel->setText(ISBN);
        nameLabel->setText(QString::fromStdString(book.book_name()));
        unitPrice_ = book.price_;
        quantityEdit->setValue(1);
        updateTotalPrice();
        connect(buttonBox, &QDialogButtonBox::accepted, [this, ISBN]() {
            int quantity = quantityEdit->value();
            if (quantity <= 0) {
                qDebug() << "错误: 购买数量必须大于0!";
                errorLabel->setText("错误: 购买数量必须大于0!");
                errorLabel->show();
                return;
            }

            Book book = book_manager->find_ISBN(string_to_array<20>(ISBN.toStdString()))[0];
            if (book.quant_ < quantity) {
                qDebug() << "错误: 库存不足!";
                errorLabel->setText("错误: 库存不足!");
                errorLabel->show();
                return;
            }

            double cost = 0.00;
            book_manager->buy(string_to_array<20>(ISBN.toStdString()), quantity, cost);

            std::string msg = current_time() + " [BUY]User " + account_manager->current_user() + " bought " + quantityEdit->text().toStdString() + " book(s). ISBN:" + ISBN.toStdString();
            log_manager->add_log(msg);
            log_manager->add_finance_log(cost);

            qDebug() << "购买成功! 总价:" << QString::number(cost, 'f', 2);
            QMessageBox::information(this, "购买成功", "购买成功! 总价: " + QString::number(cost, 'f', 2));
            
            accept();
        });

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    QLabel *ISBNLabel;
    QLabel *nameLabel;
    QSpinBox *quantityEdit;
    QLabel *priceLabel;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;
    double unitPrice_ = 0.0;

    void setupUI() {
        setWindowTitle("购买图书");
        setFixedSize(300, 250);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        ISBNLabel = new QLabel();
        nameLabel = new QLabel();
        quantityEdit = new QSpinBox();
        quantityEdit->setRange(1, 1000000);
        quantityEdit->setValue(1);
        priceLabel = new QLabel();
        priceLabel->setObjectName("priceLabel");

        formLayout->addRow("ISBN:", ISBNLabel);
        formLayout->addRow("书名:", nameLabel);
        formLayout->addRow("数量:", quantityEdit);
        formLayout->addRow("总价:", priceLabel);

        errorLabel = new QLabel();
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->hide();

        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(quantityEdit, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int){ updateTotalPrice(); });

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

    void updateTotalPrice() {
        int qty = quantityEdit->value();
        double total = unitPrice_ * qty;
        priceLabel->setText(QString::number(total, 'f', 2));
    }

};

#endif