#include <util/Logger.h>
#include <ZincArguments.h>
#include <iostream>
#include <mutex>

namespace zinc {

std::mutex m_loggerMutex;

void Logger::info(std::string const& message) {
    time_t now = time(0);
    tm* tm = localtime(&now);
    m_loggerMutex.lock();
    std::cout << "[" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << "] [" << m_loggerName << "/INFO] " << message << "\n";
    m_loggerMutex.unlock();
}
void Logger::warning(std::string const& message) {
    time_t now = time(0);
    tm* tm = localtime(&now);
    m_loggerMutex.lock();
    std::cout << "[" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << "] [" << m_loggerName << "/WARNING] " << message << "\n";
    m_loggerMutex.unlock();
}
void Logger::error(std::string const& message, bool isFatal) {
    time_t now = time(0);
    tm* tm = localtime(&now);
    m_loggerMutex.lock();
    std::cout << "[" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << "] [" << m_loggerName << "/ERROR] " << message << "\n";
    if (isFatal) exit(1);
    m_loggerMutex.unlock();
}
void Logger::debug(std::string const& message) {
    if (!g_zincArguments.m_debug) return;
    time_t now = time(0);
    tm* tm = localtime(&now);
    m_loggerMutex.lock();
    std::cout << "[" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << "] [" << m_loggerName << "/DEBUG] " << message << "\n";
    m_loggerMutex.unlock();
}

}