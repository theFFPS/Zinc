#include <util/Logger.h>
#include <ZincArguments.h>
#include <iostream>
#include <mutex>

namespace zinc {

std::mutex m_loggerMutex;

void log(std::string const& message, std::string const& type, std::string const& loggerName) {
    time_t now = time(0);
    tm* tm = localtime(&now);
    m_loggerMutex.lock();
    std::cout << "[" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << "] [" << loggerName << "/" + type + "] " 
              << message << "\n";
    m_loggerMutex.unlock();
}
void Logger::info(std::string const& message) {
    log(message, "INFO", m_loggerName);
}
void Logger::warning(std::string const& message) {
    log(message, "WARNING", m_loggerName);
}
void Logger::error(std::string const& message, bool isFatal) {
    log(message, "ERROR", m_loggerName);
    if (isFatal) exit(1);
}
void Logger::debug(std::string const& message) {
    if (!g_zincArguments.m_debug) return;
    log(message, "DEBUG", m_loggerName);
}

}