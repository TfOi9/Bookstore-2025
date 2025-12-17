#include "qt_common.hpp"
#include "globals.hpp"
#include "main_window.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataDir.isEmpty()) {
        dataDir = QDir::homePath() + "/.BookStore";
    }
    QDir().mkpath(dataDir);
    std::string base = dataDir.toStdString();

    account_manager = new AccountManager(base + "/account.dat");
    book_manager = new BookManager(base);
    log_manager = new LogManager(base);

#ifdef Q_OS_MAC
    {
        QString iconPath = QCoreApplication::applicationDirPath() + "/../Resources/BookShelf.icns";
        app.setWindowIcon(QIcon(iconPath));
    }
#endif
#ifdef Q_OS_WIN
    {
        app.setWindowIcon(QIcon(QLatin1String(":/icons/BookShelf.ico")));
    }
#endif

    MainWindow window;
    window.show();
    
    int result = app.exec();

    delete account_manager;
    delete book_manager;
    delete log_manager;

    return result;
}