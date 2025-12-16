#include "book_control_panel.hpp"
#include "add_book_dialog.hpp"
#include "buy_dialog.hpp"
#include "globals.hpp"
#include "modify_book_dialog.hpp"
#include "search_book_dialog.hpp"
#include "utils.hpp"

BookControlPanel::BookControlPanel(QWidget *parent) : QWidget(parent) {
    addButton = new QPushButton("添加", this);
    buyButton = new QPushButton("购买", this);
    modifyButton = new QPushButton("修改", this);
    importButton = new QPushButton("进货", this);
    searchButton = new QPushButton("搜索", this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch(1);
    layout->addWidget(addButton);
    layout->addWidget(buyButton);
    layout->addWidget(modifyButton);
    layout->addWidget(importButton);
    layout->addWidget(searchButton);
    
    setLayout(layout);

    applyStyle();

    connect(addButton, &QPushButton::clicked, this, &BookControlPanel::onAddButtonClicked);
    connect(buyButton, &QPushButton::clicked, this, &BookControlPanel::onBuyButtonClicked);
    connect(modifyButton, &QPushButton::clicked, this, &BookControlPanel::onModifyButtonClicked);
    connect(importButton, &QPushButton::clicked, this, &BookControlPanel::onImportButtonClicked);
    connect(searchButton, &QPushButton::clicked, this, &BookControlPanel::onSearchButtonClicked);
    
    refreshPanel();
}

void BookControlPanel::onBookSelected(const QString& ISBN) {
    qDebug() << "Book selected:" << ISBN;
    currentSelectedISBN = ISBN;
}

void BookControlPanel::onAddButtonClicked() {
    qDebug() << "Add button clicked";
    AddBookDialog dialog(this);
    dialog.exec();
    emit bookListChanged();
}

void BookControlPanel::onBuyButtonClicked() {
    qDebug() << "Buy button clicked";
    if (currentSelectedISBN.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择图书！");
        return;
    }
    BuyDialog dialog(currentSelectedISBN, this);
    dialog.exec();
    emit bookListChanged();
}

void BookControlPanel::onModifyButtonClicked() {
    qDebug() << "Modify button clicked";
    if (currentSelectedISBN.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择图书！");
        return;
    }
    Book selectedBook = book_manager->find_ISBN(string_to_array<20>(currentSelectedISBN.toStdString()))[0];
    ModifyBookDialog dialog(selectedBook, this);
    dialog.exec();
    emit bookListChanged();
}

void BookControlPanel::onImportButtonClicked() {
    qDebug() << "Import button clicked";
    if (currentSelectedISBN.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择图书！");
        return;
    }
    ImportDialog dialog(currentSelectedISBN, this);
    dialog.exec();
    emit bookListChanged();
}

void BookControlPanel::onSearchButtonClicked() {
    qDebug() << "Search button clicked";
    SearchBookDialog dialog(this);
    dialog.exec();
}

void BookControlPanel::refreshPanel() {
    switch (account_manager->current_privilege()) {
        case 0:
            addButton->hide();
            buyButton->hide();
            modifyButton->hide();
            importButton->hide();
            searchButton->hide();
            break;
        case 1:
            addButton->hide();
            buyButton->show();
            modifyButton->hide();
            importButton->hide();
            searchButton->show();
            break;
        case 3:
        case 7:
            addButton->show();
            buyButton->show();
            modifyButton->show();
            importButton->show();
            searchButton->show();
            break;
    }
}

void BookControlPanel::applyStyle() {
    setStyleSheet(R"(        
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

QString BookControlPanel::selectedISBN() const {
    return currentSelectedISBN;
}