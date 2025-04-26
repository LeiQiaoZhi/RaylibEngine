//
// Created by Qiaozhi Lei on 1/20/25.
//

#include "Logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>

void Logger::Log(std::string message, LogLevel level) {
    LogEntry entry;
    entry.message = std::move(message);
    entry.level = level;

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    entry.timestamp = std::localtime(&now_c);

    std::ostringstream oss;
    oss << "[" << entry.GetTimestampString() << "] " << entry.message;
    std::cout << oss.str() << std::endl;

    log_entries_.push_back(entry);
}
