#pragma once

#include <string>

namespace haley {
class LogLevel {
public:
    enum Level { UNKNOW, DEBUG, INFO, WARN, ERROR, FATAL };

    static const char *ToString(LogLevel::Level level);
    static LogLevel::Level FromString(const std::string& str);
};
}
