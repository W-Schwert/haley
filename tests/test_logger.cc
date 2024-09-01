#include <iostream>

#include "event.h"
#include "formatter.h"
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

int main() {
    // test_logLevel();
    // test_logEvent();
    test_logFormatter();
}
