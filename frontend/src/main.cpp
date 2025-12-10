#include "qt_common.hpp"
#include "globals.hpp"
#include "main_window.hpp"

int main(int argc, char *argv[]) {
    account_manager = new AccountManager();
    book_manager = new BookManager();
    log_manager = new LogManager();

    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();
    
    int result = app.exec();

    delete account_manager;
    delete book_manager;
    delete log_manager;

    return result;
}