#ifndef LOG_TABLE_HPP
#define LOG_TABLE_HPP

#include <vector>
#include "qt_common.hpp"
#include "globals.hpp"

class LogTable : public QTableWidget {
    Q_OBJECT
public:
    LogTable(QWidget* parent = nullptr);

    void refreshTable(bool update = 1);

    void handleSearch(const std::vector<Log>& logs);

public slots:
    void updateLogs(const std::vector<Log>& logs);

private:
    std::vector<Log> logs_;

};

#endif