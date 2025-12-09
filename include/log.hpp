#ifndef LOG_HPP
#define LOG_HPP

#include <array>
#include <vector>
#include <utility>
#include "memory_river.hpp"
#include "hash_map.hpp"
#include "utils.hpp"

class FinanceLog {
    friend class LogManager;
private:
    int count_;
    double cost_;
    // + for income, - for expenditure
public:
    FinanceLog(int count = 0, double cost = 0.00);

    ~FinanceLog() = default;

    int count() const;

    double cost() const;
};

class EmployeeLog {
    friend class LogManager;
private:
    std::array<char, 30> user_id_;
    std::array<char, 200> msg_;
public:
    EmployeeLog(const std::string& user_id = "", const std::string& msg = "");

    ~EmployeeLog() = default;

    std::string user_id() const;

    std::string msg() const;

    bool operator==(const EmployeeLog& oth) const;
};

class Log {
    friend class LogManager;
private:
    std::array<char, 200> msg_;
public:
    Log(const std::string& msg = "");

    ~Log() = default;

    std::string msg() const;
};

class LogManager {
private:
    MemoryRiver<FinanceLog> finance_file_;
    HashMap<std::array<char, 30>, EmployeeLog> employee_file_;
    MemoryRiver<Log> log_file_;
    // int current_count_;
public:
    LogManager();

    ~LogManager() = default;

    void add_finance_log(double cost);

    void add_employee_log(const std::string& user_id, const std::string& msg);

    void add_log(const std::string& msg);

    std::pair<double, double> finance(int count = -1);

    int finance_size();

    void report_finance();

    void report_employee(const std::string& user_id);

    void log();
};

#endif