#include <iostream>

#include "event.h"

int main() {
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
