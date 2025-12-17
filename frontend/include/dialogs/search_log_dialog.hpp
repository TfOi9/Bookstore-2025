#ifndef SEARCH_LOG_DIALOG_HPP
#define SEARCH_LOG_DIALOG_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "validator.hpp"
#include "log_control_panel.hpp"

class SearchLogDialog : public QDialog {
public:
    SearchLogDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
        applyStyle();
        connect(buttonBox, &QDialogButtonBox::accepted, [this, p = qobject_cast<LogControlPanel*>(parent)]() {
            QString searchType = comboBox->currentText();
            QString searchContent = searchEdit->text();
            qDebug() << "搜索日志信息";
            qDebug() << "搜索类型:" << searchType;
            qDebug() << "搜索内容:" << searchContent;

            if (searchType != "全部日志" && searchContent.isEmpty()) {
                qDebug() << "错误: 搜索内容不能为空!";
                errorLabel->setText("错误: 搜索内容不能为空!");
                errorLabel->show();
                return;
            }

            if (searchType == "全部日志") {
                if (p) {
                    emit p->searchedLogs(log_manager->serialize());
                }
            }
            else if (searchType == "时间") {
                if (p) {
                    auto logs = log_manager->serialize();
                    std::vector<Log> filteredLogs;
                    for (const auto &log : logs) {
                        std::string log_msg = log.msg();
                        int delimiter_pos = log_msg.find('[');
                        std::string log_time = log_msg.substr(0, delimiter_pos - 1);
                        if (log_time.find(searchContent.toStdString()) != std::string::npos) {
                            filteredLogs.push_back(log);
                        }
                    }
                    emit p->searchedLogs(filteredLogs);
                }
            }
            else if (searchType == "内容") {
                if (p) {
                    auto logs = log_manager->serialize();
                    std::vector<Log> filteredLogs;
                    for (const auto &log : logs) {
                        if (log.msg().find(searchContent.toStdString()) != std::string::npos) {
                            filteredLogs.push_back(log);
                        }
                    }
                    emit p->searchedLogs(filteredLogs);
                }
            }

            accept();
        });

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

private:
    QComboBox *comboBox;
    QLineEdit *searchEdit;
    QDialogButtonBox *buttonBox;
    QLabel *errorLabel;
    void setupUI() {
        setWindowTitle("搜索日志");
        setFixedSize(300, 200);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        comboBox = new QComboBox();
        comboBox->addItems({"全部日志","时间", "内容"});
        
        connect(comboBox, &QComboBox::currentTextChanged, this, [this](const QString &text){
            if (text == "全部日志") {
                searchEdit->setEnabled(false);
            } else {
                searchEdit->setEnabled(true);
            }
        });

        searchEdit = new QLineEdit();
        searchEdit->setEnabled(false);

        formLayout->addRow("搜索类型:", comboBox);
        formLayout->addRow("搜索内容:", searchEdit);

        errorLabel = new QLabel();
        errorLabel->setStyleSheet("color: red; font-size: 12px");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->hide();

        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        mainLayout->addLayout(formLayout);
        mainLayout->addWidget(errorLabel);
        mainLayout->addWidget(buttonBox);
    }

    void applyStyle() {
        setStyleSheet(R"(
            QLabel {
                font-size: 14px;
                color: #333333;
            }
            
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

};

#endif