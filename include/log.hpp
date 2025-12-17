#ifndef LOG_HPP
#define LOG_HPP

#include <array>
#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>
#include "memory_river.hpp"
#include "hash_map.hpp"
#include "utils.hpp"

class FinanceLog {
    friend class LogManager;
private:
    int count_;
    double cost_;
    // + for income, - for expenditure
    std::array<char, 20> time_;
public:
    FinanceLog(int count = 0, double cost = 0.00, const std::string& time = "");

    ~FinanceLog() = default;

    // returns count.
    int count() const;

    // returns cost.
    double cost() const;

    // returns time.
    std::string time() const;
};

class EmployeeLog {
    friend class LogManager;
private:
    std::array<char, 30> user_id_;
    std::array<char, 200> msg_;
public:
    EmployeeLog(const std::string& user_id = "", const std::string& msg = "");

    ~EmployeeLog() = default;

    // returns user ID.
    std::string user_id() const;

    // returns message.
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

    // returns message.
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

    LogManager(const std::string& base_dir);

    ~LogManager() = default;

    // adds a finance log.
    void add_finance_log(double cost, const std::string& time = current_time());

    // adds an employee log.
    void add_employee_log(const std::string& user_id, const std::string& msg);

    // adds a general log.
    void add_log(const std::string& msg);

    // returns finance logs.
    std::pair<double, double> finance(int count = -1);

    // returns finance log size.
    int finance_size();

    // report finance logs.
    void report_finance(std::ostream& os = std::cout);

    // report employee logs.
    void report_employee(const std::string& user_id, std::ostream& os = std::cout);

    // lists all logs.
    void log(std::ostream& os = std::cout);

    // returns serialized logs.
    std::vector<Log> serialize();

    // exports data to a CSV file.
    void export_data(const std::string& filename = "log.csv");
};

#endif