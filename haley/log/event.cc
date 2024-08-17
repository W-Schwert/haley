#include "event.h"

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
}
