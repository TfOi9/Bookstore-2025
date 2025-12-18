#ifndef BOOK_CONTROL_PANEL_HPP
#define BOOK_CONTROL_PANEL_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "add_book_dialog.hpp"
#include "buy_dialog.hpp"
#include "import_dialog.hpp"

class BookControlPanel : public QWidget {
    Q_OBJECT
public:
    BookControlPanel(QWidget *parent = nullptr);

    void refreshPanel();

    QString selectedISBN() const;

signals:
    void bookListChanged();

    void searchedBooks(const std::vector<Book>& books);

private slots:
    void onAddButtonClicked();
    void onBuyButtonClicked();
    void onModifyButtonClicked();
    void onDeleteButtonClicked();
    void onImportButtonClicked();
    void onSearchButtonClicked();

public slots:
    void onBookSelected(const QString& ISBN);

private:
    QPushButton *addButton;
    QPushButton *buyButton;
    QPushButton *modifyButton;
    QPushButton *deleteButton;
    QPushButton *importButton;
    QPushButton *searchButton;

    void applyStyle();
    
    QString currentSelectedISBN;

};

#endif