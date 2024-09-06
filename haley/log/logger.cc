#include "logger.h"

#include <cstdarg>

namespace haley {
Logger::Logger(const std::string& name) : m_name(name), m_level(LogLevel::DEBUG) {
    m_formatter = std::make_shared<LogFormatter>();
}

void Logger::log(LogEvent::ptr event) {
    if (m_appenders.empty() || m_level > event->getLevel()) {
        return;
    }
    for (auto& appender : m_appenders) {
        appender->log(m_name, event);
    }
}

void Logger::addAppender(LogAppender::ptr appender) {
    appender->setFormatter(m_formatter);
    m_appenders.emplace_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it) {
        if (*it == appender) {
            m_appenders.erase(it);
            return;
        }
    }
}

void Logger::clearAppenders() {
    m_appenders.clear();
}

void Logger::setFormatter(LogFormatter::ptr formatter) {
    if (formatter->isError()) {
        return;
    }
    m_formatter = formatter;
    for (auto appender : m_appenders) {
        appender->setFormatter(m_formatter);
    }
}

void Logger::setFormatter(const std::string& pattern) {
    LogFormatter::ptr fmt = std::make_shared<LogFormatter>(pattern);
    if (!fmt->isError()) {
        setFormatter(fmt);
    }
}

LoggerWrap::LoggerWrap(Logger::ptr logger, LogEvent::ptr event)
    : m_logger(logger), m_event(event) {
}

LoggerWrap::~LoggerWrap() {
    m_logger->log(m_event);
}

LoggerManager* LoggerManager::GetInstance() {
    return Singleton<LoggerManager>::GetInstance();
}

LoggerManager::LoggerManager() {
    Logger::ptr root = std::make_shared<Logger>();
    LogAppender::ptr appender = std::make_shared<CLILogAppender>();
    root->addAppender(appender);
    m_loggers[root->getName()] = root;
}

Logger::ptr LoggerManager::getLogger(const std::string& name, bool create) {
    auto it = m_loggers.find(name);
    if (it == m_loggers.end() && !create) {
        return nullptr;
    }
    if (it != m_loggers.end()) {
        return it->second;
    }
    Logger::ptr logger = std::make_shared<Logger>(name);
    m_loggers[name] = logger;
    return logger;
}
}
