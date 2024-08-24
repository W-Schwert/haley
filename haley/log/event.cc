#include "event.h"

#include <cstdarg>
#include <cstdio>

namespace haley{
const char *LogLevel::ToString(LogLevel::Level level) {
    switch (level) {
#define XX(name) \
        case LogLevel::name: \
            return #name;
        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
#undef XX
        default:
            return "UNKNOW";
    }
}

LogLevel::Level LogLevel::FromString(const std::string& str) {
#define XX(level, v) \
    if (str == #v) { \
        return LogLevel::level; \
    }
    XX(DEBUG, DEBUG);
    XX(DEBUG, debug);
    XX(INFO, INFO);
    XX(INFO, info);
    XX(WARN, WARN);
    XX(WARN, warn);
    XX(ERROR, ERROR);
    XX(ERROR, error);
    XX(FATAL, FATAL);
    XX(FATAL, fatal);
#undef XX
    return LogLevel::UNKNOW;
}

LogEvent::LogEvent(const std::string& file, const std::string& threadName, uint32_t line,
    uint32_t threadID, uint32_t fiberID, uint32_t elapse, uint32_t time, LogLevel::Level level)
    : m_file(file), m_threadName(threadName), m_line(line), m_threadID(threadID),
      m_fiberID(fiberID), m_elapse(elapse), m_time(time), m_level(level) {}

void LogEvent::format(const char* fmt, ...) {
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}

void LogEvent::format(const char* fmt, va_list al) {
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);
    if (len != -1) {
        m_ss << std::string(buf, len);
        free(buf);
    }
}
}   // namespace haley
