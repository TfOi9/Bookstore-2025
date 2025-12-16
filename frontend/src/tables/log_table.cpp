#include "log_table.hpp"
#include <string>

LogTable::LogTable(QWidget* parent) : QTableWidget(parent) {
    setColumnCount(2);
    QStringList headers = {"时间", "内容"};
    setHorizontalHeaderLabels(headers);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setVisible(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(true);
    logs_ = log_manager->serialize();
}

void LogTable::refreshTable(bool update) {
    clearContents();
    std::vector<Log> logs;
    if (update) {
        std::string msg = current_time() + " [REPORT]User " + account_manager->current_user() + " viewed logs.";
        log_manager->add_log(msg);
        logs = log_manager->serialize();
    }
    else logs = logs_;
    logs_ = logs;
    setRowCount(logs.size());
    for (int i = 0; i < logs.size(); i++) {
        const auto& log = logs[i];
        int delimiter_pos = log.msg().find('[');
        std::string time_str = log.msg().substr(0, delimiter_pos - 1);
        std::string content_str = log.msg().substr(delimiter_pos);
        setItem(i, 0, new QTableWidgetItem(QString::fromStdString(time_str)));
        setItem(i, 1, new QTableWidgetItem(QString::fromStdString(content_str)));
    }
    resizeRowsToContents();
}

void LogTable::updateLogs(const std::vector<Log>& logs) {
    qDebug() << "Updating logs in LogTable, new size:" << logs.size();
    logs_ = logs;
    refreshTable(false);
}

void LogTable::handleSearch(const std::vector<Log>& logs) {
    qDebug() << "Handling search in LogTable, new size:" << logs.size();
    logs_ = logs;
    refreshTable(false);
}