#include "qt_common.hpp"
#include "globals.hpp"
#include "main_window.hpp"
#include <iostream>
#include <exception>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (dataDir.isEmpty()) {
            dataDir = QDir::homePath() + "/.BookStore";
        }
        
        if (!QDir(dataDir).exists()) {
            if (!QDir().mkpath(dataDir)) {
                qWarning() << "Failed to create data directory:" << dataDir;
                QMessageBox::critical(nullptr, "Error", 
                    QString("Failed to create data directory: %1").arg(dataDir));
                return 1;
            }
        }
        
        std::string base = dataDir.toStdString();
        qDebug() << "Data directory:" << dataDir;

        qDebug() << "Initializing AccountManager...";
        account_manager = new AccountManager(base + "/account.dat");
        
        qDebug() << "Initializing BookManager...";
        book_manager = new BookManager(base);
        
        qDebug() << "Initializing LogManager...";
        log_manager = new LogManager(base);

#ifdef Q_OS_MAC
        {
            QString resourcePath = QCoreApplication::applicationDirPath() + "/../Resources";
            QString iconPath = resourcePath + "/BookShelf.icns";
            qDebug() << "Looking for icon at:" << iconPath;
            
            if (QFileInfo(iconPath).exists()) {
                app.setWindowIcon(QIcon(iconPath));
                qDebug() << "Icon loaded from:" << iconPath;
            } else {
                qWarning() << "Icon file not found at:" << iconPath;
                QString altPath = QCoreApplication::applicationDirPath() + "/BookShelf.icns";
                if (QFileInfo(altPath).exists()) {
                    app.setWindowIcon(QIcon(altPath));
                    qDebug() << "Icon loaded from alternative path:" << altPath;
                }
            }
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

        // Cleanup
        if (account_manager) {
            delete account_manager;
            account_manager = nullptr;
        }
        if (book_manager) {
            delete book_manager;
            book_manager = nullptr;
        }
        if (log_manager) {
            delete log_manager;
            log_manager = nullptr;
        }

        return result;
    } catch (const std::exception &e) {
        qCritical() << "Fatal error:" << e.what();
        QMessageBox::critical(nullptr, "Fatal Error", 
            QString("An unexpected error occurred:\n%1").arg(QString::fromStdString(e.what())));
        return 1;
    } catch (...) {
        qCritical() << "Unknown fatal error";
        QMessageBox::critical(nullptr, "Fatal Error", 
            "An unknown error occurred. Please check the application logs.");
        return 1;
    }
}