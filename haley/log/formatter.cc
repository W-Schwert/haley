#include "formatter.h"

#include <functional>
#include <map>
#include <tuple>

namespace haley {
class MessageFormatItem : public FormatItem {

};

class LevelFormatItem : public FormatItem {

};

class ElapseFormatItem : public FormatItem {

};

class NameFormatItem : public FormatItem {

};

class ThreadIDFormatItem : public FormatItem {

};

class NewLineFormatItem : public FormatItem {

};

class DateTimeFormatItem : public FormatItem {

};

class FileNameFormatItem : public FormatItem {

};

class LineFormatItem : public FormatItem {

};

class TabFormatItem : public FormatItem {

};

class FiberIDFormatItem : public FormatItem {

};

class ThreadNameFormatItem : public FormatItem {

};

class StringFormatItem : public FormatItem {

};


LogFormatter::LogFormatter(const std::string& pattern) : m_pattern(pattern), m_error(false) {
    init();
}

// "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
void LogFormatter::init() {
    std::vector<std::tuple<char, std::string>> items;
    for (size_t i = 0; i < m_pattern.size(); ++i) {
        char msg = 's';
        std::string content = "";
        if (m_pattern[i] == '%' && i < m_pattern.size() - 1) {
            if (i + 1 < m_pattern.size() && m_pattern[++i] == '{' && ++i < m_pattern.size()) {
                size_t start = i;
                while (i < m_pattern.size() && m_pattern[i] != '}') {
                    i += 1;
                }
                content = m_pattern.substr(start, i - start);
            }
        } else {
            size_t start = i;
            while (i < m_pattern.size() && m_pattern[i] != '%') {
                i += 1;
            }
            content = m_pattern.substr(start, i - start);
            if (m_pattern[i] == '%') {
                i -= 1;
            }
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
        } else {
            m_items.push_back(it->second(std::get<1>(item)));
        }
    }
}
}   // namespace haley
