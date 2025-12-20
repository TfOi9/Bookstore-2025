#ifndef ADD_BOOK_DIALOG_HPP
#define ADD_BOOK_DIALOG_HPP

#include <algorithm>
#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"

class AddBookDialog : public QDialog {
public:
    AddBookDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString ISBN = ISBNEdit->text();
            QString name = nameEdit->text();
            QString author = authorEdit->text();
            QString keyword = keywordEdit->text();
            QString priceStr = priceEdit->text();

            qDebug() << "添加图书信息";
            qDebug() << "ISBN:" << ISBN;
            qDebug() << "书名:" << name;
            qDebug() << "作者:" << author;
            qDebug() << "关键词:" << keyword;
            qDebug() << "价格:" << priceStr;

            if (ISBN.isEmpty()) {
                qDebug() << "错误: ISBN为必填项!";
                errorLabel->setText("错误: ISBN为必填项!");
                errorLabel->show();
                return;
            }

            if (priceStr.isEmpty()) {
                qDebug() << "错误: 价格为必填项!";
                errorLabel->setText("错误: 价格为必填项!");
                errorLabel->show();
                return;
            }

            if (!Validator(ISBN.toStdString()).max_len(20).visible_only()) {
                qDebug() << "错误: ISBN格式不正确!";
                errorLabel->setText("错误: ISBN格式不正确!");
                errorLabel->show();
                return;
            }

            if (book_manager->count_ISBN(string_to_array<20>(ISBN.toStdString()))) {
                qDebug() << "错误: ISBN已存在!";
                errorLabel->setText("错误: ISBN已存在!");
                errorLabel->show();
                return;
            }

            if (!UnicodeValidator(name.toStdString()).max_len(60).han().no_quotes() &&
                !UnicodeValidator(name.toStdString()).max_len(60).japanese().no_quotes()) {
                qDebug() << "错误: 书名格式不正确!";
                errorLabel->setText("错误: 书名格式不正确!");
                errorLabel->show();
                return;
            }

            if (!UnicodeValidator(author.toStdString()).max_len(60).han().no_quotes() &&
                !UnicodeValidator(author.toStdString()).max_len(60).japanese().no_quotes()) {
                qDebug() << "错误: 作者格式不正确!";
                errorLabel->setText("错误: 作者格式不正确!");
                errorLabel->show();
                return;
            }

            if (!UnicodeValidator(keyword.toStdString()).max_len(60).han().no_quotes() &&
                !UnicodeValidator(keyword.toStdString()).max_len(60).japanese().no_quotes()) {
                qDebug() << "错误: 关键词格式不正确!";
                errorLabel->setText("错误: 关键词格式不正确!");
                errorLabel->show();
                return;
            }

            if (keyword.size()) {
                auto key_words = parse_keywords(string_to_array<240>(keyword.toStdString()));
                if (keyword.isEmpty() != 0 && key_words.empty()) {
                    qDebug() << "错误: 关键词格式不正确!";
                    errorLabel->setText("错误: 关键词格式不正确!");
                    errorLabel->show();
                    return;
                }
                std::sort(key_words.begin(), key_words.end());
                if (keyword.isEmpty() != 0 && std::unique(key_words.begin(), key_words.end()) != key_words.end()) {
                    qDebug() << "错误: 关键词格式不正确!";
                    errorLabel->setText("错误: 关键词格式不正确!");
                    errorLabel->show();
                    return;
                }
            }

            if (!Validator(priceStr.toStdString()).max_len(13).number_and_dot_only().only_one_dot()) {
                qDebug() << "错误: 价格格式不正确!";
                errorLabel->setText("错误: 价格格式不正确!");
                errorLabel->show();
                return;
            }
            
            double price = 0.00;
            try {
                price = std::stod(priceStr.toStdString());
            }
            catch (...) {
                qDebug() << "错误: 价格格式不正确!";
                errorLabel->setText("错误: 价格格式不正确!");
                errorLabel->show();
                return;
            }

            if (price <= 0.00) {
                qDebug() << "错误: 价格必须大于0!";
                errorLabel->setText("错误: 价格必须大于0!");
                errorLabel->show();
                return;
            }

            Book new_book(ISBN.toStdString(), name.toStdString(), author.toStdString(), keyword.toStdString(), price, 0, book_manager->size());
            book_manager->add(new_book);

            std::string msg = current_time() + " [SELECT]User " + account_manager->current_user() + " selected book " + ISBN.toStdString() + '.';
            log_manager->add_log(msg);

            msg = current_time() + " [MODIFY]User " + account_manager->current_user() + " modified book " + ISBN.toStdString() + '.';
            log_manager->add_log(msg);
            log_manager->add_employee_log(account_manager->current_user(), msg);

            accept();
        });

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    QLineEdit *ISBNEdit;
    QLineEdit *nameEdit;
    QLineEdit *authorEdit;
    QLineEdit *keywordEdit;
    QLineEdit *priceEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("添加图书");
        setFixedSize(400, 300);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        ISBNEdit = new QLineEdit();
        nameEdit = new QLineEdit();
        authorEdit = new QLineEdit();
        keywordEdit = new QLineEdit();
        priceEdit = new QLineEdit();
        
        formLayout->addRow("ISBN:", ISBNEdit);
        formLayout->addRow("书名:", nameEdit);
        formLayout->addRow("作者:", authorEdit);
        formLayout->addRow("关键词:", keywordEdit);
        formLayout->addRow("价格:", priceEdit);

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