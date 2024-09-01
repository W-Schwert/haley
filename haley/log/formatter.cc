#include "formatter.h"

#include <functional>
#include <map>
#include <tuple>

namespace haley {
class MessageFormatItem : public FormatItem {
public:
    MessageFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << event->getContent();
    }
};

class LevelFormatItem : public FormatItem {
public:
    LevelFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << LogLevel::ToString(event->getLevel());
    }
};

class ElapseFormatItem : public FormatItem {
public:
    ElapseFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << event->getElapse();
    }
};

class NameFormatItem : public FormatItem {
public:
    NameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << logger_name;
    }
};

class ThreadIDFormatItem : public FormatItem {
public:
    ThreadIDFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << event->getThreadID();
    }
};

class NewLineFormatItem : public FormatItem {
public:
    NewLineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << std::endl;
    }
};

class DateTimeFormatItem : public FormatItem {
public:
    DateTimeFormatItem(const std::string& pattern = "") :m_pattern(pattern) {
        if (m_pattern.empty()) {
            m_pattern = "%Y-%m-%d %H:%M:%S";
        }
    }

    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_pattern.c_str(), &tm);
        os << buf;
    }

private:
    std::string m_pattern;
};

class FileNameFormatItem : public FormatItem {
public:
    FileNameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << event->getFile();
    }
};

class LineFormatItem : public FormatItem {
public:
    LineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << event->getLine();
    }
};

class TabFormatItem : public FormatItem {
public:
    TabFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << "\t";
    }
};

class FiberIDFormatItem : public FormatItem {
public:
    FiberIDFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << event->getFiberID();
    }
};

class ThreadNameFormatItem : public FormatItem {
public:
    ThreadNameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << event->getThreadName();
    }
};

class StringFormatItem : public FormatItem {
public:
    StringFormatItem(const std::string& str = "") : m_string(str) {}
    void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) override {
        os << m_string;
    }

private:
    std::string m_string;
};


LogFormatter::LogFormatter(const std::string& pattern) : m_pattern(pattern), m_error(false) {
    init();
}

std::string LogFormatter::format(const std::string& logger_name, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto item : m_items) {
       item->format(ss, logger_name, event);
    }
    return ss.str();
}

std::ostream& LogFormatter::format(std::ostream& ofs,
    const std::string& logger_name, LogEvent::ptr event) {
    for (auto item : m_items) {
        item->format(ofs, logger_name, event);
    }
    return ofs;
}

// "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
void LogFormatter::init() {
    std::vector<std::tuple<char, std::string>> items;
    for (size_t i = 0; i < m_pattern.size(); ++i) {
        char msg = 's';
        std::string content = "";
        if (m_pattern[i] == '%' && i + 1 < m_pattern.size()) {
            if (m_pattern[i + 1] == '%') {
                items.push_back(std::make_pair('s', m_pattern.substr(++i, 1)));
                continue;
            }
            msg = m_pattern[++i];
            if (i + 1 < m_pattern.size() && m_pattern[i + 1] == '{' && i + 2 < m_pattern.size()) {
                size_t start = i + 2;
                while (i < m_pattern.size() && m_pattern[i] != '}') {
                    i += 1;
                }
                content = m_pattern.substr(start, i - start);
            }
        } else {
            size_t start = i;
            while (i + 1 < m_pattern.size() && m_pattern[i + 1] != '%') {
                i += 1;
            }
            content = m_pattern.substr(start, i - start + 1);
        }
        items.push_back(std::make_pair(msg, content));
    }

    static std::map<char, std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(c, item) \
        {#c[0], [](const std::string& str){ \
            return std::static_pointer_cast<FormatItem>(std::make_shared<item>(str)); \
        }}
        XX(m, MessageFormatItem),
        XX(p, LevelFormatItem),
        XX(r, ElapseFormatItem),
        XX(c, NameFormatItem),
        XX(t, ThreadIDFormatItem),
        XX(n, NewLineFormatItem),
        XX(d, DateTimeFormatItem),
        XX(f, FileNameFormatItem),
        XX(l, LineFormatItem),
        XX(T, TabFormatItem),
        XX(F, FiberIDFormatItem),
        XX(N, ThreadNameFormatItem),
        XX(s, StringFormatItem)
#undef XX
    };

    for (auto item : items) {
        auto it = s_format_items.find(std::get<0>(item));
        if (it == s_format_items.end()) {
            m_items.push_back(std::make_shared<StringFormatItem>(
                "<<error_format %" + std::string(1, std::get<0>(item)) + ">>"));
            m_error = true;
        } else {
            m_items.push_back(it->second(std::get<1>(item)));
        }
    }
}
}   // namespace haley
