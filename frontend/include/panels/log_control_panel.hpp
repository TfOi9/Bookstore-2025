#ifndef LOG_CONTROL_PANEL_HPP
#define LOG_CONTROL_PANEL_HPP

#include "qt_common.hpp"
#include "globals.hpp"

class LogControlPanel : public QWidget {
    Q_OBJECT
public:
    LogControlPanel(QWidget *parent = nullptr);

    void refreshPanel();

signals:
    void refreshLogs();

    void searchedLogs(const std::vector<Log>& logs);

private slots:
    void onRefreshButtonClicked();
    void onFinanceQueryButtonClicked();
    void onReportFinanceButtonClicked();
    void onReportEmployeeButtonClicked();
    void onExportCSVButtonClicked();
    void onSearchButtonClicked();

private:
    QPushButton *refreshButton;
    QPushButton *financeQueryButton;
    QPushButton *reportFinanceButton;
    QPushButton *reportEmployeeButton;
    QPushButton *exportCSVButton;
    QPushButton *searchButton;

    void applyStyle();
    
};

#endif