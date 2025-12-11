#ifndef BOOK_LIST_WIDGET_HPP
#define BOOK_LIST_WIDGET_HPP

#include "qt_common.hpp"
#include "book.hpp" 

class BookTableWidget : public QWidget {
    Q_OBJECT
public:
    explicit BookTableWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
        setupTable();
        applyStyle();
        refreshBooks();
    }
    
    void refreshBooks();
    
signals:
    void bookSelected(const Book &book);
    void editRequested(const Book &book);
    
private slots:
    void onSearchClicked();
    void onAddClicked();
    void onEditClicked();
    void onItemDoubleClicked(QTableWidgetItem *item);
    
private:
    void setupUI() {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setSpacing(10);
        
        QHBoxLayout *searchLayout = new QHBoxLayout();
        searchEdit = new QLineEdit(this);
        searchEdit->setPlaceholderText("搜索书籍（书名、作者、ISBN、关键词）...");
        searchEdit->setClearButtonEnabled(true);
        
        searchButton = new QPushButton("搜索", this);
        connect(searchButton, &QPushButton::clicked, this, &BookTableWidget::onSearchClicked);
        
        searchLayout->addWidget(searchEdit);
        searchLayout->addWidget(searchButton);
        searchLayout->addStretch();
        
        mainLayout->addLayout(searchLayout);
        
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        addButton = new QPushButton("添加书籍", this);
        editButton = new QPushButton("编辑", this);
        
        connect(addButton, &QPushButton::clicked, this, &BookTableWidget::onAddClicked);
        connect(editButton, &QPushButton::clicked, this, &BookTableWidget::onEditClicked);
        
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(editButton);
        buttonLayout->addStretch();
        
        mainLayout->addLayout(buttonLayout);
        
        table = new QTableWidget(this);
        mainLayout->addWidget(table);
    }
    void setupTable() {
        QStringList headers;
        headers << "ISBN" << "书名" << "作者" << "关键词" << "价格" << "库存" << "描述";
        table->setColumnCount(headers.size());
        table->setHorizontalHeaderLabels(headers);
        
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setAlternatingRowColors(true);
        table->verticalHeader()->setVisible(false);
        table->horizontalHeader()->setStretchLastSection(true);
        
        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
        
        connect(table, &QTableWidget::itemDoubleClicked, this, &BookTableWidget::onItemDoubleClicked);
    }
    void loadBooks(const std::vector<Book> &books) {

    }
    void applyStyle() {
        setStyleSheet(R"(
            BookTableWidget {
                background-color: #ffffff;
            }
            
            QTableWidget {
                border: 1px solid #e0e0e0;
                border-radius: 4px;
                gridline-color: #f0f0f0;
                font-size: 13px;
            }
            
            QTableWidget::item {
                padding: 8px;
                border-bottom: 1px solid #f0f0f0;
            }
            
            QTableWidget::item:selected {
                background-color: #e3f2fd;
                color: #1976d2;
            }
            
            QHeaderView::section {
                background-color: #f5f5f5;
                padding: 12px 8px;
                border: none;
                border-right: 1px solid #e0e0e0;
                font-weight: bold;
                color: #333333;
            }
            
            QLineEdit {
                padding: 8px 12px;
                border: 1px solid #e0e0e0;
                border-radius: 4px;
                font-size: 13px;
            }
            
            QPushButton {
                padding: 8px 16px;
                border: 1px solid #e0e0e0;
                border-radius: 4px;
                background-color: #ffffff;
                color: #333333;
                font-size: 13px;
            }
            
            QPushButton:hover {
                background-color: #f5f5f5;
            }
            
            QPushButton:disabled {
                background-color: #f9f9f9;
                color: #999999;
            }
        )");
    }
    
    QTableWidget *table;
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QPushButton *addButton;
    QPushButton *editButton;
    
    int currentPrivilege = 0;
};

#endif