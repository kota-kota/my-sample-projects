#include "LogWriter.hpp"
#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <cstdio>
#include <ctime>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

namespace
{
std::string currentDateTime()
{
    std::time_t t = std::time(nullptr);
    std::tm now;
#if WIN32
    (void)::localtime_s(&now, &t);
#else
    localtime_r(&now, &t);
#endif

    auto epoch_time = system_clock::now().time_since_epoch();
    auto sec_since_epoch = duration_cast<seconds>(epoch_time).count();
    auto millisec_since_epoch = duration_cast<milliseconds>(epoch_time).count();

    int32_t millisec = static_cast<int32_t>(millisec_since_epoch - (sec_since_epoch * 1000));

    char datetime_sec[128];
    strftime(datetime_sec, sizeof(datetime_sec), "%m-%d-%Y %H:%M:%S", &now);

    char datetime_millisec[128];
    snprintf(datetime_millisec, sizeof(datetime_millisec), "%s.%03d", datetime_sec, millisec);
    return datetime_millisec;
}
} // namespace

namespace fw
{
class LogWriter::Impl
{
  public:
    Impl() : logpath_("./")
    {
    }

    void setLogPath(const std::string logpath)
    {
        logpath_ = logpath;
    }

    void write(const std::string level, const std::string tag, const std::string log)
    {
        std::string datetime = currentDateTime();
        printf("%s [%s][%s] %s\n", datetime.c_str(), level.c_str(), tag.c_str(), log.c_str());
    }

  private:
    std::string logpath_;
};
} // namespace fw

namespace fw
{
LogWriter::LogWriter() : impl_(new (std::nothrow) Impl)
{
}

LogWriter::~LogWriter()
{
    if (impl_ != nullptr)
    {
        delete impl_;
    }
}

void LogWriter::setLogPath(const std::string logpath)
{
    if (impl_ != nullptr)
    {
        impl_->setLogPath(logpath);
    }
}

void LogWriter::write(const std::string level, const std::string tag, const std::string log)
{
    if (impl_ != nullptr)
    {
        impl_->write(level, tag, log);
    }
}
} // namespace fw
