#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <vector>
#include <raygui.h>


enum class LogLevel {
    Info,
    Warning,
    Error
};

struct LogEntry {
    std::string message;
    std::tm *timestamp;
    LogLevel level = LogLevel::Info;

    std::string GetTimestampString() const {
        char buffer[80];
        strftime(buffer, 80, "%H:%M:%S", timestamp);
        return std::string(buffer);
    }

    int GetIcon() const {
        switch (level) {
            case LogLevel::Info:
                return ICON_INFO;
            case LogLevel::Warning:
                return ICON_WARNING;
            case LogLevel::Error:
                return ICON_DEMON;
        }
        return ICON_NONE;
    }
};


class Logger {
public:
    Logger() = default;

    ~Logger() = default;

    void Log(std::string message, LogLevel level = LogLevel::Info);

    void Warn(const std::string &message) {
        Log(message, LogLevel::Warning);
    }

    void Error(const std::string &message) {
        Log(message, LogLevel::Error);
    }

    const std::vector<LogEntry> &GetLogEntries() const {
        return log_entries_;
    }

private:
    std::vector<LogEntry> log_entries_;
};


#endif //LOGGER_H
