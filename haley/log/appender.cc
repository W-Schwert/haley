#include "appender.h"

#include <iostream>

namespace haley {
void LogAppender::setFormatter(LogFormatter::ptr formatter) {
    m_formatter = formatter;
}

LogFormatter::ptr LogAppender::getFormatter() const {
    return m_formatter;
}

void CLILogAppender::log(const std::string& logger_name, LogEvent::ptr event) {
    m_formatter->format(std::cout, logger_name, event);
}

FileLogAppender::FileLogAppender(const std::string& filename) : m_filename(filename) {
    init();
}

FileLogAppender::~FileLogAppender() {
    if (m_filestream) {
        m_filestream.close();
    }
}

void FileLogAppender::log(const std::string& logger_name, LogEvent::ptr event) {
    m_formatter->format(m_filestream, logger_name, event);
}

bool FileLogAppender::init() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename, std::ios::app);
    return m_filestream.is_open();
}
}   // namespace haley
