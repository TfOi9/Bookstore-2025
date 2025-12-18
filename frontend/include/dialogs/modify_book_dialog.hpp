#ifndef MODIFY_BOOK_DIALOG_HPP
#define MODIFY_BOOK_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"

class ModifyBookDialog : public QDialog {
public:
    ModifyBookDialog(const Book &book, QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();

        // Pre-fill fields with existing book data
        ISBNEdit->setText(QString::fromStdString(book.ISBN()));
        nameEdit->setText(QString::fromStdString(book.book_name()));
        authorEdit->setText(QString::fromStdString(book.author()));
        keywordEdit->setText(QString::fromStdString(book.keyword()));
        priceEdit->setText(QString::number(book.price_, 'f', 2));

        connect(buttonBox, &QDialogButtonBox::accepted, [this, book]() {
            QString ISBN = ISBNEdit->text();
            QString name = nameEdit->text();
            QString author = authorEdit->text();
            QString keyword = keywordEdit->text();
            QString priceStr = priceEdit->text();

            qDebug() << "修改图书信息";
            qDebug() << "ISBN:" << ISBN;
            qDebug() << "书名:" << name;
            qDebug() << "作者:" << author;
            qDebug() << "关键词:" << keyword;
            qDebug() << "价格:" << priceStr;

            if (ISBN.isEmpty() || name.isEmpty() || author.isEmpty() || keyword.isEmpty() || priceStr.isEmpty()) {
                qDebug() << "错误: 所有字段均为必填项!";
                errorLabel->setText("错误: 所有字段均为必填项!");
                errorLabel->show();
                return;
            }

            if (!Validator(ISBN.toStdString()).max_len(20).visible_only()) {
                qDebug() << "错误: ISBN格式不正确!";
                errorLabel->setText("错误: ISBN格式不正确!");
                errorLabel->show();
                return;
            }

            if (ISBN.toStdString() != book.ISBN() && book_manager->count_ISBN(string_to_array<20>(ISBN.toStdString()))) {
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

            std::array<char, 20> cur_ISBN = book.ISBN_;
            if (ISBN.toStdString() != book.ISBN()) {
                book_manager->modify_ISBN(cur_ISBN, string_to_array<20>(ISBN.toStdString()));
                cur_ISBN = string_to_array<20>(ISBN.toStdString());
            }
            if (name.toStdString() != book.book_name()) {
                book_manager->modify_book_name(cur_ISBN, string_to_array<240>(name.toStdString()));
            }
            if (author.toStdString() != book.author()) {
                book_manager->modify_author(cur_ISBN, string_to_array<240>(author.toStdString()));
            }
            if (keyword.toStdString() != book.keyword()) {
                book_manager->modify_keyword(cur_ISBN, string_to_array<240>(keyword.toStdString()));
            }
            if (price != book.price_) {
                book_manager->modify_price(cur_ISBN, price);
            }

            qDebug() << "图书修改成功!";

            std::string msg = current_time() + " [MODIFY]User " + account_manager->current_user() + " modified book " + book.ISBN() + '.';
            log_manager->add_log(msg);
            log_manager->add_employee_log(account_manager->current_user(), msg);
            
            accept();
        });
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
        setWindowTitle("修改图书");
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