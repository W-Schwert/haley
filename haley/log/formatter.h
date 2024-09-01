#pragma once

#include <memory>
#include <string>
#include <vector>

#include "event.h"

namespace haley {
class FormatItem {
public:
    typedef std::shared_ptr<FormatItem> ptr;
    virtual ~FormatItem() {}
    virtual void format(std::ostream& os, const std::string& logger_name, LogEvent::ptr event) = 0;
};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    /**
     * @brief 构造函数
     * @param[in] pattern 格式模板
     * @details
     * m, MessageFormatItem             //m:消息
     * p, LevelFormatItem               //p:日志级别
     * r, ElapseFormatItem              //r:累计毫秒数
     * t, ThreadIdFormatItem            //t:线程id
     * n, NewLineFormatItem             //n:换行
     * d, DateTimeFormatItem            //d:时间
     * f, FilenameFormatItem            //f:文件名
     * l, LineFormatItem                //l:行号
     * T, TabFormatItem                 //T:Tab
     * F, FiberIdFormatItem             //F:协程id
     * N, ThreadNameFormatItem          //N:线程名称
     * s, StringFormatItem              //s:直接输出给定字符串
     * c, NameFormatItem                //c:日志器名称 （未实现，因为Logger还未实现）
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
    LogFormatter(const std::string& pattern =
        "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");

    std::string format(const std::string& logger_name, LogEvent::ptr event);
    std::ostream& format(std::ostream& ofs, const std::string& logger_name, LogEvent::ptr event);
    bool isError() const { return m_error; }
    const std::string getPattern() const { return m_pattern; }

private:
    void init();

    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error;
};
}   // namespace haley
