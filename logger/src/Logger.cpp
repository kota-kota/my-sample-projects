#include "Logger.hpp"
#include "LoggerImpl.hpp"
#include <cstdarg>
#include <cstdio>

namespace fw
{
LoggerOptions::LoggerOptions() : logpath_("./")
{
}

LoggerOptions &LoggerOptions::withLogPath(const std::string logpath)
{
    logpath_ = logpath;
    return *this;
}

LoggerOptions &LoggerOptions::withFilteringTags(const std::vector<std::string> tags)
{
    tags_ = tags;
    return *this;
}

void LoggerOptions::apply()
{
    LoggerImpl::getInstance().setLogPath(logpath_);
    LoggerImpl::getInstance().setFilteringTags(tags_);
}
} // namespace fw

namespace fw
{
void Logger::output(const std::string level, const std::string tag, const char *const format, ...)
{
    std::string strlog;
    {
        va_list va;
        va_start(va, format);

        // 実際の文字数を取得する(終端NULL文字は含まない)
        char tmp[8];
        int32_t size = std::vsnprintf(tmp, sizeof(tmp), format, va);

        // 実際の文字数分のメモリを確保し取得する
        int32_t bufsize = size + 1;
        char *buf = new (std::nothrow) char[bufsize];
        if (buf == nullptr)
        {
            return;
        }
        std::vsnprintf(buf, bufsize, format, va);
        va_end(va);
        strlog = buf;
        delete[] buf;
    }
    LoggerImpl::getInstance().output(level, tag, strlog);
}
} // namespace fw
