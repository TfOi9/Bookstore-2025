#include "main_window.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    topBar(nullptr), stackedTable(nullptr), stackedPanel(nullptr),
    accountTable(nullptr), bookTable(nullptr), logTable(nullptr),
    accountControlPanel(nullptr), bookControlPanel(nullptr), logControlPanel(nullptr),
    initialized_(false) {
    
    try {
        initializeUI();
        // Schedule initialization after event loop starts to ensure managers are ready
        QTimer::singleShot(100, this, &MainWindow::initializeComponents);
    } catch (const std::exception &e) {
        qCritical() << "Failed to initialize UI:" << e.what();
        QMessageBox::critical(this, "Initialization Error", 
            QString("Failed to initialize application:\n%1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        qCritical() << "Unknown error during UI initialization";
        QMessageBox::critical(this, "Initialization Error", 
            "An unknown error occurred during initialization.");
    }
}

void MainWindow::initializeUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    topBar = new TopBar(this);
    mainLayout->addWidget(topBar);

    connect(topBar, &TopBar::authChanged, this, &MainWindow::handleAuthChanged);

    connect(topBar, &TopBar::accountButtonClicked, this, [this]{
        if (accountTable && stackedTable) {
            stackedTable->setCurrentWidget(accountTable);
            stackedPanel->setCurrentWidget(accountControlPanel);
            accountTable->refreshTable();
            accountControlPanel->refreshPanel();
        }
    });
    
    connect(topBar, &TopBar::bookButtonClicked, this, [this]{
        if (bookTable && stackedTable) {
            stackedTable->setCurrentWidget(bookTable);
            stackedPanel->setCurrentWidget(bookControlPanel);
            bookTable->refreshTable();
            bookControlPanel->refreshPanel();
        }
    });

    connect(topBar, &TopBar::logButtonClicked, this, [this]{
        if (logTable && stackedTable) {
            stackedTable->setCurrentWidget(logTable);
            stackedPanel->setCurrentWidget(logControlPanel);
            logTable->refreshTable();
        }
    });

    stackedTable = new QStackedWidget(centralWidget);
    stackedPanel = new QStackedWidget(centralWidget);

    // Create placeholder widgets while waiting for deferred initialization
    QWidget *placeholderTable = new QWidget(stackedTable);
    QWidget *placeholderPanel = new QWidget(stackedPanel);
    
    stackedTable->addWidget(placeholderTable);
    stackedPanel->addWidget(placeholderPanel);

    mainLayout->addWidget(stackedTable, 1);
    mainLayout->addWidget(stackedPanel, 0);
    stackedPanel->setFixedHeight(60);

    statusBar()->showMessage(QStringLiteral("游客模式"));
    
    resize(800, 600);
    setWindowTitle("BookStore");

    setCentralWidget(centralWidget);
}

void MainWindow::initializeComponents() {
    if (initialized_) return;
    
    try {
        qDebug() << "Starting deferred component initialization...";
        
        accountTable = new AccountTable(stackedTable);
        bookTable = new BookTable(stackedTable);
        logTable = new LogTable(stackedTable);

        accountControlPanel = new AccountControlPanel(stackedPanel);
        bookControlPanel = new BookControlPanel(stackedPanel);
        logControlPanel = new LogControlPanel(stackedPanel);

        connect(accountTable, &AccountTable::accountSelected, accountControlPanel, &AccountControlPanel::onAccountSelected);
        connect(bookTable, &BookTable::bookSelected, bookControlPanel, &BookControlPanel::onBookSelected);

        stackedTable->addWidget(accountTable);
        stackedTable->addWidget(bookTable);
        stackedTable->addWidget(logTable);
        // stackedTable->setCurrentWidget(bookTable);

        stackedPanel->addWidget(accountControlPanel);
        stackedPanel->addWidget(bookControlPanel);
        stackedPanel->addWidget(logControlPanel);
        // stackedPanel->setCurrentWidget(bookControlPanel);

        connect(accountControlPanel, &AccountControlPanel::accountListChanged, this, [this]{
            if (accountTable) accountTable->refreshTable();
        });

        connect(bookControlPanel, &BookControlPanel::bookListChanged, this, [this]{
            if (bookTable) bookTable->refreshTable();
        });

        connect(logControlPanel, &LogControlPanel::refreshLogs, this, [this]{
            if (logTable) logTable->refreshTable(true);
        });

        connect(accountControlPanel, &AccountControlPanel::searchedAccounts, accountTable, &AccountTable::handleSearch);
        connect(bookControlPanel, &BookControlPanel::searchedBooks, bookTable, &BookTable::handleSearch);
        connect(logControlPanel, &LogControlPanel::searchedLogs, logTable, &LogTable::handleSearch);

        connect(accountControlPanel, &AccountControlPanel::loginSucceeded, this, [this](const QString &userId){
            topBar->refreshBar();
            handleAuthChanged(QStringLiteral("已登录: ") + userId);
        });

        topBar->refreshBar();
        
        qDebug() << "Component initialization completed successfully";
        initialized_ = true;
    } catch (const std::exception &e) {
        qCritical() << "Error during component initialization:" << e.what();
        QMessageBox::critical(this, "Initialization Error", 
            QString("Failed to initialize components:\n%1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        qCritical() << "Unknown error during component initialization";
        QMessageBox::critical(this, "Initialization Error", 
            "An unknown error occurred during component initialization.");
    }
}

void MainWindow::handleAuthChanged(const QString &msg) {
    if (bookTable && stackedTable) {
        stackedTable->setCurrentWidget(bookTable);
        stackedPanel->setCurrentWidget(bookControlPanel);
        bookTable->refreshTable();
        bookControlPanel->refreshPanel();
        setStatusMessage(msg);
    }
}
