#pragma once

#include <memory>
#include <unordered_map>

#include "appender.h"
#include "event.h"
#include "formatter.h"
#include "singleton.h"
#include "utils.h"

#define HALEY_LOG_LEVEL(logger, level) \
    haley::LoggerWrap(logger, std::make_shared<haley::LogEvent>(__FILE__, "threadName", __LINE__, \
        haley::GetThreadID(), haley::GetFiberID(), haley::GetRunTime(), time(0), level)) \
        .getEvent()->getSS()

#define HALEY_LOG_DEBUG(logger) HALEY_LOG_LEVEL(logger, haley::LogLevel::DEBUG)
#define HALEY_LOG_INFO(logger) HALEY_LOG_LEVEL(logger, haley::LogLevel::INFO)
#define HALEY_LOG_WARN(logger) HALEY_LOG_LEVEL(logger, haley::LogLevel::WARN)
#define HALEY_LOG_ERROR(logger) HALEY_LOG_LEVEL(logger, haley::LogLevel::ERROR)
#define HALEY_LOG_FATAL(logger) HALEY_LOG_LEVEL(logger, haley::LogLevel::FATAL)

#define HALEY_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    haley::LoggerWrap(logger, std::make_shared<haley::LogEvent>(__FILE__, "threadName", __LINE__, \
        haley::GetThreadID(), haley::GetFiberID(), haley::GetRunTime(), time(0), level)) \
        .getEvent()->format(fmt, __VA_ARGS__)

#define HALEY_LOG_FMT_DEBUG(logger, fmt, ...) HALEY_LOG_FMT_LEVEL(logger, haley::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define HALEY_LOG_FMT_INFO(logger, fmt, ...) HALEY_LOG_FMT_LEVEL(logger, haley::LogLevel::INFO, fmt, __VA_ARGS__)
#define HALEY_LOG_FMT_WARN(logger, fmt, ...) HALEY_LOG_FMT_LEVEL(logger, haley::LogLevel::WARN, fmt, __VA_ARGS__)
#define HALEY_LOG_FMT_ERROR(logger, fmt, ...) HALEY_LOG_FMT_LEVEL(logger, haley::LogLevel::ERROR, fmt, __VA_ARGS__)
#define HALEY_LOG_FMT_FATAL(logger, fmt, ...) HALEY_LOG_FMT_LEVEL(logger, haley::LogLevel::FATAL, fmt, __VA_ARGS__)

#define HALEY_LOG_ROOT() haley::LoggerManager::GetInstance()->getLogger("root")
#define HALEY_LOG_NAME(name) haley::LoggerManager::GetInstance()->getLogger(name)

namespace haley {
class Logger {
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string& name = "root");
    void log(LogEvent::ptr event);
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAppenders();
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level level) { m_level = level; }
    const std::string& getName() const { return m_name; }
    void setFormatter(LogFormatter::ptr formatter);
    void setFormatter(const std::string& pattern);

private:
    std::string m_name;
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
    std::vector<LogAppender::ptr> m_appenders;
};

class LoggerWrap {
public:
    LoggerWrap(Logger::ptr logger, LogEvent::ptr event);
    ~LoggerWrap();
    LogEvent::ptr getEvent() const { return m_event; }

private:
    Logger::ptr m_logger;
    LogEvent::ptr m_event;
};

class LoggerManager {
friend class Singleton<LoggerManager>;
public:
    static LoggerManager* GetInstance();
    Logger::ptr getLogger(const std::string& name, bool create = true);

private:
    LoggerManager();

private:
    std::unordered_map<std::string, Logger::ptr> m_loggers;
};
}
