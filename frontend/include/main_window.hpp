#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "login_dialog.hpp"
#include "register_dialog.hpp"
#include "profile_dialog.hpp"
#include "book_table.hpp"
#include "book_control_panel.hpp"
#include "top_bar.hpp"
#include <functional>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        topBar = new TopBar(this);
        mainLayout->addWidget(topBar);

        connect(topBar, &TopBar::authChanged, this, [this](const QString &s){
            bookTable->refreshTable();
            bookControlPanel->refreshPanel();
            setStatusMessage(s);
        });
        
        connect(topBar, &TopBar::bookButtonClicked, this, [this]{
            stackedTable->setCurrentWidget(bookTable);
            bookTable->refreshTable();
            bookControlPanel->refreshPanel();
        });

        stackedTable = new QStackedWidget(centralWidget);
        stackedPanel = new QStackedWidget(centralWidget);

        bookTable = new BookTable(stackedTable);
        // userTable = new UserTable(stackedTable);
        bookControlPanel = new BookControlPanel(stackedPanel);

        stackedTable->addWidget(bookTable);
        // stackedTable->addWidget(userTable);
        stackedTable->setCurrentWidget(bookTable);

        stackedPanel->addWidget(bookControlPanel);
        stackedPanel->setCurrentWidget(bookControlPanel);

        mainLayout->addWidget(stackedTable, 1);
        mainLayout->addWidget(stackedPanel, 0);
        stackedPanel->setFixedHeight(60);

        statusBar()->showMessage(QStringLiteral("游客模式"));
        
        resize(800, 600);
        setWindowTitle("BookStore");

        topBar->refreshBar();

        setCentralWidget(centralWidget);
    }

    void setStatusMessage(const QString &msg, int timeout = 0) {
        statusBar()->showMessage(msg, timeout);
    }


private:
    TopBar *topBar;
    QStackedWidget *stackedTable;
    BookTable *bookTable;
    // UserTable *userTable;
    QStackedWidget *stackedPanel;
    BookControlPanel *bookControlPanel;

};

#endif