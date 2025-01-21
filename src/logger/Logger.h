#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <vector>

struct LogEntry {
    std::string message;
    std::string timestamp;
};

class Logger {
public:
    Logger() = default;

    ~Logger() = default;

    void Log(std::string message);

    const std::vector<LogEntry> &GetLogEntries() const {
        return log_entries_;
    }

private:
    std::vector<LogEntry> log_entries_;
};


#endif //LOGGER_H
