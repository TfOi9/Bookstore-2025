#include "../include/log.hpp"

FinanceLog::FinanceLog(int count, double cost, const std::string& time) : count_(count), cost_(cost), time_(string_to_array<20>(time)) {}

int FinanceLog::count() const {
    return count_;
}

double FinanceLog::cost() const {
    return cost_;
}

std::string FinanceLog::time() const {
    return array_to_string<20>(time_);
}

EmployeeLog::EmployeeLog(const std::string& user_id, const std::string& msg) {
    user_id_ = string_to_array<30>(user_id);
    msg_ = string_to_array<200>(msg);
}

std::string EmployeeLog::user_id() const {
    return array_to_string<30>(user_id_);
}

std::string EmployeeLog::msg() const {
    return array_to_string<200>(msg_);
}

bool EmployeeLog::operator==(const EmployeeLog& oth) const {
    return (user_id_ == oth.user_id_ && msg_ == oth.msg_);
}

Log::Log(const std::string& msg) {
    msg_ = string_to_array<200>(msg);
}

std::string Log::msg() const {
    return array_to_string<200>(msg_);
}

LogManager::LogManager() : finance_file_("finance.dat"), employee_file_("employee.dat"), log_file_("log.dat") {
    finance_file_.initialise();
    log_file_.initialise();
}

void LogManager::add_finance_log(double cost, const std::string& time) {
    FinanceLog finance_log(finance_file_.size(), cost, time);
    finance_file_.write(finance_log);
}

void LogManager::add_employee_log(const std::string& user_id, const std::string& msg) {
    EmployeeLog employee_log(user_id, msg);
    employee_file_.insert(string_to_array<30>(user_id), employee_log);
}

void LogManager::add_log(const std::string& msg) {
    Log log(msg);
    log_file_.write(log);
}

std::pair<double, double> LogManager::finance(int count) {
    double income = 0.00, expenditure = 0.00;
    int pos = 0;
    if (count >= 0) {
        pos = finance_file_.size() - count;
    }
    for (int i = pos; i < finance_file_.size(); i++) {
        FinanceLog finance_log;
        finance_file_.read(finance_log, i);
        if (finance_log.cost_ > 0) {
            income += finance_log.cost_;
        }
        else {
            expenditure -= finance_log.cost_;
        }
    }
    return std::make_pair(income, expenditure);
}

int LogManager::finance_size() {
    return finance_file_.size();
}

void LogManager::report_finance() {
    double total_income = 0.00, total_expenditure = 0.00;
    for (int i = 0; i < finance_file_.size(); i++) {
        FinanceLog finance_log;
        finance_file_.read(finance_log, i);
        if (finance_log.cost_ > 0) {
            total_income += finance_log.cost_;
        }
        else {
            total_expenditure -= finance_log.cost_;
        }
        std::cout << finance_log.count() + 1 << ' ' << finance_log.time() << ' ' << (finance_log.cost() < 0 ? '-' : '+') << ' ' << std::fixed << std::setprecision(2) << std::abs(finance_log.cost()) << '\n';
    }
    std::cout << "Total Income: " << std::fixed << std::setprecision(2) << total_income << '\n';
    std::cout << "Total Expenditure: " << std::fixed << std::setprecision(2) << total_expenditure << '\n';
}

void LogManager::report_employee(const std::string& user_id) {
    std::array<char, 30> arr = string_to_array<30>(user_id);
    std::vector<EmployeeLog> logs = employee_file_.find(arr);
    if (!logs.empty()) {
        std::cout << "Employee " << user_id << ":\n";
    }
    for (auto log : logs) {
        std::cout << log.msg() << '\n';
    }
}

void LogManager::log() {
    for (int i = 0; i < log_file_.size(); i++) {
        Log log;
        log_file_.read(log, i);
        std::cout << log.msg() << '\n';
    }
}

std::vector<Log> LogManager::serialize() {
    std::vector<Log> logs;
    for (int i = 0; i < log_file_.size(); i++) {
        Log log;
        log_file_.read(log, i);
        logs.push_back(log);
    }
    return logs;
}