#include <iostream>

#include "appender.h"
#include "event.h"
#include "formatter.h"
#include "logger.h"
#include "utils.h"

void test_logLevel() {
#define XX(level) \
    std::cout << haley::LogLevel::ToString(haley::LogLevel::FromString(#level)) << std::endl;
    XX(DEBUG);
    XX(debug);
    XX(INFO);
    XX(info);
    XX(WARN);
    XX(warn);
    XX(FATAL);
    XX(fatal);
    XX(UNKNOW);
    XX(unknow);
    XX(bogus);
#undef XX
}

void test_logEvent() {
    haley::LogEvent::ptr event = std::make_shared<haley::LogEvent>(__FILE__, "threadName", __LINE__,
        haley::GetThreadID(), haley::GetFiberID(), haley::GetRunTime(), time(0), haley::LogLevel::DEBUG);
    event->format("test format %s.", "wan");
    std::cout << event->getContent() << " " << event->getFile() << " " << event->getLine() << " "
        << haley::LogLevel::ToString(event->getLevel()) << " " << event->getElapse() << " "
        << event->getTime() << std::endl;
}

void test_logFormatter() {
    haley::LogEvent::ptr event = std::make_shared<haley::LogEvent>(__FILE__, "threadName", __LINE__,
        haley::GetThreadID(), haley::GetFiberID(), haley::GetRunTime(), time(0), haley::LogLevel::DEBUG);
    event->format("test format %s.", "wan");

    haley::LogFormatter::ptr formatter = std::make_shared<haley::LogFormatter>("%d{%Y-%m-%d %H:%M:%S}%T[%p]%T%f:%l%T%m%n");
    haley::LogFormatter::ptr formatter_1 = std::make_shared<haley::LogFormatter>();
    std::cout << formatter->format("test_logFormatter", event);
    formatter->format(std::cout, "test_logFormatter", event);
    std::cout << formatter_1->format("test_logFormatter", event);
    formatter_1->format(std::cout, "test_logFormatter", event);
}

void test_logAppender() {
    haley::LogEvent::ptr event = std::make_shared<haley::LogEvent>(__FILE__, "threadName", __LINE__,
        haley::GetThreadID(), haley::GetFiberID(), haley::GetRunTime(), time(0), haley::LogLevel::DEBUG);
    event->format("test format %s.", "wan");

    haley::LogFormatter::ptr formatter = std::make_shared<haley::LogFormatter>();
    haley::LogFormatter::ptr formatter_1 = std::make_shared<haley::LogFormatter>("%d{%Y-%m-%d %H:%M:%S}%T[%p]%T%f:%l%T%m%n");
    haley::LogAppender::ptr cil_appender = std::make_shared<haley::CLILogAppender>();
    haley::FileLogAppender::ptr file_appender = std::make_shared<haley::FileLogAppender>("test_log.txt");
    cil_appender->setFormatter(formatter);
    file_appender->setFormatter(formatter_1);
    cil_appender->log("test cil log appender", event);
    file_appender->log("test file log appender", event);
}

void test_logger() {
    haley::LogEvent::ptr event = std::make_shared<haley::LogEvent>(__FILE__, "threadName", __LINE__,
        haley::GetThreadID(), haley::GetFiberID(), haley::GetRunTime(), time(0), haley::LogLevel::DEBUG);
    event->format("test format %s.", "wan");

    haley::LogAppender::ptr cil_appender = std::make_shared<haley::CLILogAppender>();
    haley::FileLogAppender::ptr file_appender = std::make_shared<haley::FileLogAppender>("test_log.txt");

    haley::Logger::ptr logger = std::make_shared<haley::Logger>("test_logger");
    logger->addAppender(cil_appender);
    logger->addAppender(file_appender);

    logger->log(event);

    logger->delAppender(file_appender);
    logger->log(event);
    logger->addAppender(file_appender);

    haley::LogFormatter::ptr fmt = std::make_shared<haley::LogFormatter>("%d{%Y-%m-%d %H:%M:%S}%T[%p]%T%f:%l%T%m%n");
    logger->setFormatter(fmt);
    logger->log(event);

    fmt = std::make_shared<haley::LogFormatter>("%l%o%j%iu%dsfatgf%fadfas[sfadfasfad]%n");
    logger->setFormatter(fmt);
    logger->log(event);

    logger->setFormatter("%l%o%j%iu%dsfatgf%fadfas[sfadfasfad]%n");
    logger->log(event);
}

void test_logger_macro() {
    HALEY_LOG_DEBUG(HALEY_LOG_ROOT()) << "test HALEY_LOG_DEBUG";
    HALEY_LOG_FMT_DEBUG(HALEY_LOG_ROOT(), "test HALEY_LOG_FMT_DEBUG %s", "test go on");

    haley::FileLogAppender::ptr file_appender = std::make_shared<haley::FileLogAppender>("test_log.txt");
    haley::Logger::ptr logger = HALEY_LOG_NAME("");
    logger->addAppender(file_appender);
    HALEY_LOG_DEBUG(HALEY_LOG_NAME("")) << "test HALEY_LOG_DEBUG";
    HALEY_LOG_FMT_DEBUG(HALEY_LOG_NAME(""), "test HALEY_LOG_FMT_DEBUG %s", "test go on");
}

int main() {
    // test_logLevel();
    // test_logEvent();
    // test_logFormatter();
    // test_logAppender();
    // test_logger();
    test_logger_macro();
}
