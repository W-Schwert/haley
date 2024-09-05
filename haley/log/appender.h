#pragma once

#include <fstream>
#include <memory>

#include "event.h"
#include "formatter.h"

namespace haley {
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;

    virtual ~LogAppender() {}
    virtual void log(const std::string& logger_name, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr formatter);
    LogFormatter::ptr getFormatter() const;

protected:
    LogFormatter::ptr m_formatter;
};

class CLILogAppender : public LogAppender {
public:
    typedef std::shared_ptr<CLILogAppender> ptr;

    virtual void log(const std::string& logger_name, LogEvent::ptr event) override;
};

class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;

    FileLogAppender(const std::string& filename);
    ~FileLogAppender();
    virtual void log(const std::string& logger_name, LogEvent::ptr event) override;

private:
    bool init();

private:
    std::string m_filename;
    std::ofstream m_filestream;
};
}   // namespace haley
