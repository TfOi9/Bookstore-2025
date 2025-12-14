#ifndef BOOK_CONTROL_PANEL_HPP
#define BOOK_CONTROL_PANEL_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "add_book_dialog.hpp"

class BookControlPanel : public QWidget {
    Q_OBJECT
public:
    BookControlPanel(QWidget *parent = nullptr);

    void refreshPanel();

signals:
    void bookListChanged();

private slots:
    void onAddButtonClicked();
    void onBuyButtonClicked();
    void onModifyButtonClicked();
    void onImportButtonClicked();
    void onSearchButtonClicked();

private:
    QPushButton *addButton;
    QPushButton *buyButton;
    QPushButton *modifyButton;
    QPushButton *importButton;
    QPushButton *searchButton;

    void applyStyle();

};

#endif