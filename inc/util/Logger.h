#pragma once

#include <string>

namespace zinc {

struct Logger {
private:
    std::string m_loggerName;
public:
    Logger() : m_loggerName("") {}
    Logger(std::string const& loggerName) : m_loggerName(loggerName) {}

    void info(std::string const& message);
    void warning(std::string const& message);
    void error(std::string const& message, bool isFatal = false);
    void debug(std::string const& message);
};

}