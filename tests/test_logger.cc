#include <iostream>

#include "event.h"

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
        0, 0, 0, time(0), haley::LogLevel::DEBUG);
    event->format("test format %s.", "wan");
    std::cout << event->getContent() << " " << event->getFile() << " " << event->getLine() << " "
        << haley::LogLevel::ToString(event->getLevel()) << std::endl;
}

int main() {
    test_logLevel();
    test_logEvent();
}
