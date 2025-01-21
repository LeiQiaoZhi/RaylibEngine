//
// Created by Qiaozhi Lei on 1/20/25.
//

#include "Logger.h"

#include <iostream>

void Logger::Log(std::string message) {
    LogEntry entry;
    entry.message = std::move(message);

    std::cout << entry.message << std::endl;
    log_entries_.push_back(entry);
}
