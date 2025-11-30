#ifndef LOG_HPP
#define LOG_HPP

#include <array>
#include <vector>
#include <utility>
#include "memory_river.hpp"

class FinanceLog {
    friend class LogManager;
private:
    std::array<char, 30> user_id_;
    std::array<char, 20> ISBN_;
    int count_;
    double cost_;
    // + for income, - for outcome
public:
    FinanceLog(int count, const std::string& user_id, const std::string& ISBN, double cost);

    ~FinanceLog() = default;

    std::string user_id() const;

    std::string ISBN() const;

    int count() const;

    double cost() const;
};

class Log {
    friend class LogManager;
private:
    std::array<char, 200> msg_;
public:
    Log(const std::string& msg);

    ~Log() = default;

    std::string msg() const;
};

class LogManager {
private:
    MemoryRiver<FinanceLog> finance_file_;
    MemoryRiver<Log> log_file_;
    int current_count_;
public:
    LogManager();

    ~LogManager() = default;

    void add_finance_log(const std::string& user_id, const std::string& ISBN, double cost);

    void add_log(const std::string& msg);

    std::pair<double, double> finance();

    void report_employee(const std::string& user_id);

    void log();
};

#endif