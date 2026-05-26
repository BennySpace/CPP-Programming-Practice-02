#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <iostream>
#include <mutex>
#include <string>

inline std::mutex& console_mutex() {
    static std::mutex mutex;
    return mutex;
}

inline void log_line(const std::string& message) {
    std::lock_guard<std::mutex> lock(console_mutex());
    std::cout << message << std::endl;
}

#endif // CONSOLE_UTILS_H
