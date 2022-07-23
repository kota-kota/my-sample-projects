#include "LoggerImpl.hpp"

namespace fw
{
LoggerImpl &LoggerImpl::getInstance()
{
    static LoggerImpl impl;
    return impl;
}

void LoggerImpl::setLogPath(const std::string logpath)
{
    writer_.setLogPath(logpath);
}

void LoggerImpl::setFilteringTags(const std::vector<std::string> tags)
{
    tags_ = tags;
}

void LoggerImpl::output(const std::string level, const std::string tag, const std::string log)
{
    for (std::string filter_tag : tags_)
    {
        if (tag == filter_tag)
        {
            return;
        }
    }
    writer_.write(level, tag, log);
}

LoggerImpl::LoggerImpl() : writer_(), tags_()
{
}
} // namespace fw
