#ifndef SEARCH_BOOK_DIALOG_HPP
#define SEARCH_BOOK_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"
#include "book_control_panel.hpp"

class SearchBookDialog : public QDialog {
public:
    SearchBookDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this, p = qobject_cast<BookControlPanel*>(parent)]() {
            QString searchType = comboBox->currentText();
            QString searchContent = searchEdit->text();
            qDebug() << "搜索图书信息";
            qDebug() << "搜索类型:" << searchType;
            qDebug() << "搜索内容:" << searchContent;

            if (searchType != "全部图书" && searchContent.isEmpty()) {
                qDebug() << "错误: 搜索内容不能为空!";
                errorLabel->setText("错误: 搜索内容不能为空!");
                errorLabel->show();
                return;
            }

            if (searchType == "全部图书") {
                if (p) {
                    emit p->searchedBooks(book_manager->serialize());
                }
            }
            else if (searchType == "ISBN") {
                if (!Validator(searchContent.toStdString()).max_len(20).visible_only()) {
                    qDebug() << "错误: ISBN格式不正确!";
                    errorLabel->setText("错误: ISBN格式不正确!");
                    errorLabel->show();
                    return;
                }
                if (p) {
                    auto books = book_manager->find_ISBN(string_to_array<20>(searchContent.toStdString()));
                    emit p->searchedBooks(books);
                }
            }
            else if (searchType == "书名") {
                if (!ChineseValidator(searchContent.toStdString()).max_len(60).han().no_quotes()) {
                    qDebug() << "错误: 书名格式不正确!";
                    errorLabel->setText("错误: 书名格式不正确!");
                    errorLabel->show();
                    return;
                }
                if (p) {
                    auto books = book_manager->find_book_name(string_to_array<240>(searchContent.toStdString()));
                    emit p->searchedBooks(books);
                }
            }
            else if (searchType == "作者") {
                if (!ChineseValidator(searchContent.toStdString()).max_len(60).han().no_quotes()) {
                    qDebug() << "错误: 作者格式不正确!";
                    errorLabel->setText("错误: 作者格式不正确!");
                    errorLabel->show();
                    return;
                }
                if (p) {
                    auto books = book_manager->find_author(string_to_array<240>(searchContent.toStdString()));
                    emit p->searchedBooks(books);
                }
            }
            else if (searchType == "关键词") {
                if (!ChineseValidator(searchContent.toStdString()).max_len(60).han().no_quotes().no_pipes()) {
                    qDebug() << "错误: 关键词格式不正确!";
                    errorLabel->setText("错误: 关键词格式不正确!");
                    errorLabel->show();
                    return;
                }
                if (p) {
                    auto books = book_manager->find_keyword(string_to_array<240>(searchContent.toStdString()));
                    emit p->searchedBooks(books);
                }
            }

            accept();
        });

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    QComboBox *comboBox;
    QLineEdit *searchEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;
    void setupUI() {
        setWindowTitle("搜索图书");
        setFixedSize(300, 200);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();
        
        comboBox = new QComboBox();
        comboBox->addItems({"全部图书","ISBN", "书名", "作者", "关键词"});

        connect(comboBox, &QComboBox::currentTextChanged, this, [this](const QString &text){
            if (text == "全部图书") {
                searchEdit->setEnabled(false);
            } else {
                searchEdit->setEnabled(true);
            }
        });
        
        searchEdit = new QLineEdit();
        searchEdit->setEnabled(false);
        
        formLayout->addRow("搜索类型:", comboBox);
        formLayout->addRow("搜索内容:", searchEdit);

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