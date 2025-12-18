#ifndef DELETE_BOOK_DIALOG_HPP
#define DELETE_BOOK_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"

class DeleteBookDialog : public QDialog {
public:
    DeleteBookDialog(const QString& ISBN, QWidget* parent = nullptr) {
        setupUI();
        applyStyle();
        ISBNEdit->setText(ISBN);
        connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
            QString delISBN = ISBNEdit->text();

            qDebug() << "删除账户信息";
            qDebug() << "用户ID:" << delISBN;

            if (delISBN.isEmpty()) {
                qDebug() << "错误: ISBN为必填项!";
                errorLabel->setText("错误: ISBN为必填项!");
                errorLabel->show();
                return;
            }

            if (book_manager->count_ISBN(string_to_array<20>(delISBN.toStdString())) == 0) {
                qDebug() << "错误: 该图书不存在!";
                errorLabel->setText("错误: 该图书不存在!");
                errorLabel->show();
                return;
            }

            bool success = book_manager->delete_book(string_to_array<20>(delISBN.toStdString()));
            if (!success) {
                qDebug() << "错误: 删除图书失败!";
                errorLabel->setText("错误: 删除图书失败!");
                errorLabel->show();
                return;
            }

            std::string msg = current_time() + " [BOOKDEL]Deleted book " + delISBN.toStdString() + '.';
            log_manager->add_log(msg);

            accept();
        });
    }

private:
    QLineEdit *ISBNEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;

    void setupUI() {
        setWindowTitle("删除图书");
        setFixedSize(300, 150);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        ISBNEdit = new QLineEdit(this);
        formLayout->addRow("ISBN:", ISBNEdit);

        errorLabel = new QLabel(this);
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->setText("警告：删除图书后不可恢复！");

        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

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