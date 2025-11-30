#ifndef LOG_HPP
#define LOG_HPP

#include <array>
#include <vector>
#include <utility>
#include "memory_river.hpp"

class Log {
    friend class LogManager;
private:
    std::array<char, 30> user_id_;
    std::array<char, 20> ISBN_;
    int count_;
    double cost_;
    // + for income, - for outcome
public:
    Log(int count, const std::string& user_id, const std::string& ISBN, double cost);

    ~Log() = default;

    std::string user_id() const;

    std::string ISBN() const;

    int count() const;

    double cost() const;
};

class LogManager {
private:
    MemoryRiver<Log> log_file_;
    int current_count_;
public:
    LogManager(const std::string& file_name = "log.dat");

    std::pair<double, double> finance();

    void report_employee(const std::string& user_id);

    void log();
};

#endif