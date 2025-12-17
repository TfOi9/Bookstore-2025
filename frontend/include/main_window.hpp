#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "log.hpp"
#include "qt_common.hpp"
#include "globals.hpp"
#include "login_dialog.hpp"
#include "register_dialog.hpp"
#include "profile_dialog.hpp"
#include "account_table.hpp"
#include "account_control_panel.hpp"
#include "book_table.hpp"
#include "book_control_panel.hpp"
#include "log_table.hpp"
#include "log_control_panel.hpp"
#include "top_bar.hpp"
#include <functional>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    void setStatusMessage(const QString &msg, int timeout = 0) {
        statusBar()->showMessage(msg, timeout);
    }

private slots:
    void initializeComponents();

private:
    void initializeUI();
    void handleAuthChanged(const QString &msg);

    TopBar *topBar;

    QStackedWidget *stackedTable;
    AccountTable *accountTable;
    BookTable *bookTable;
    LogTable *logTable;

    QStackedWidget *stackedPanel;
    AccountControlPanel *accountControlPanel;
    BookControlPanel *bookControlPanel;
    LogControlPanel *logControlPanel;
    
    bool initialized_;
};

#endif