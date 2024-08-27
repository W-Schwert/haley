#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace haley {
class LogLevel {
public:
    enum Level { UNKNOW, DEBUG, INFO, WARN, ERROR, FATAL };

    static const char *ToString(LogLevel::Level level);
    static LogLevel::Level FromString(const std::string& str);
};

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;

    LogEvent(const std::string& file, const std::string& threadName, uint32_t line,
        uint32_t threadID, uint32_t fiberID, uint32_t elapse, uint32_t time, LogLevel::Level level);
    const std::string& getFile() const { return m_file; }
    const std::string& getThreadName() const { return m_threadName; }
    uint32_t getLine() const { return m_line; }
    uint32_t getThreadID() const { return m_threadID; }
    uint32_t getFiberID() const { return m_fiberID; }
    uint32_t getElapse() const { return m_elapse; }
    uint64_t getTime() const { return m_time; }
    std::string getContent() const { return m_ss.str(); }
    LogLevel::Level getLevel() const { return m_level; }
    std::stringstream& getSS() { return m_ss; }
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
    ~LogEvent() {}

private:
    std::string m_file;         // 文件名
    std::string m_threadName;   // 线程名
    uint32_t m_line;            // 行号
    uint32_t m_threadID;         // 线程ID
    uint32_t m_fiberID;         // 协程ID
    uint32_t m_elapse;          // 流逝的时间，单位ms
    uint64_t m_time;            // 时间戳
    std::stringstream m_ss;     // 日志内容流
    LogLevel::Level m_level;    // 日志等级
};
}   // namespace haley
