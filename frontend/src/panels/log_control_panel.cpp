#include "log_control_panel.hpp"

LogControlPanel::LogControlPanel(QWidget *parent) : QWidget(parent) {
    refreshButton = new QPushButton("刷新", this);
    financeQueryButton = new QPushButton("财务查询", this);
    reportFinanceButton = new QPushButton("生成财务报表", this);
    reportEmployeeButton = new QPushButton("生成员工报表", this);
    exportCSVButton = new QPushButton("导出CSV", this);
    searchButton = new QPushButton("搜索", this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch(1);
    layout->addWidget(refreshButton);
    layout->addWidget(financeQueryButton);
    layout->addWidget(reportFinanceButton);
    layout->addWidget(reportEmployeeButton);
    layout->addWidget(exportCSVButton);
    layout->addWidget(searchButton);
    
    setLayout(layout);

    applyStyle();

    connect(refreshButton, &QPushButton::clicked, this, &LogControlPanel::onRefreshButtonClicked);
    connect(financeQueryButton, &QPushButton::clicked, this, &LogControlPanel::onFinanceQueryButtonClicked);
    connect(reportFinanceButton, &QPushButton::clicked, this, &LogControlPanel::onReportFinanceButtonClicked);
    connect(reportEmployeeButton, &QPushButton::clicked, this, &LogControlPanel::onReportEmployeeButtonClicked);
    connect(exportCSVButton, &QPushButton::clicked, this, &LogControlPanel::onExportCSVButtonClicked);
    connect(searchButton, &QPushButton::clicked, this, &LogControlPanel::onSearchButtonClicked);
}

void LogControlPanel::onRefreshButtonClicked() {
    qDebug() << "Refresh logs button clicked";
    emit refreshLogs();
}

void LogControlPanel::onFinanceQueryButtonClicked() {
    qDebug() << "Finance query button clicked";
}

void LogControlPanel::onReportFinanceButtonClicked() {
    qDebug() << "Report finance button clicked";
}

void LogControlPanel::onReportEmployeeButtonClicked() {
    qDebug() << "Report employee button clicked";
}

void LogControlPanel::onExportCSVButtonClicked() {
    qDebug() << "Export CSV button clicked";
}

void LogControlPanel::onSearchButtonClicked() {
    qDebug() << "Search logs button clicked";
}

void LogControlPanel::applyStyle() {
    setStyleSheet(R"(        
        QPushButton {
            background-color: transparent;
            border: none;
            color: #666666;
            font-size: 14px;
            padding: 8px 16px;
            border-radius: 4px;
        }
        
        QPushButton:hover {
            background-color: #f5f5f5;
            color: #333333;
        }
    )");
}