#include "LogWriter.hpp"
#include "Directory.hpp"
#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <cstdio>
#include <ctime>
#include <fstream>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

namespace
{
class DateTime
{
  public:
    DateTime()
    {
        std::time_t t = std::time(nullptr);
#if WIN32
        (void)::localtime_s(&now_, &t);
#else
        localtime_r(&now_, &t);
#endif

        auto epoch_time = system_clock::now().time_since_epoch();
        auto sec_since_epoch = duration_cast<seconds>(epoch_time).count();
        auto millisec_since_epoch = duration_cast<milliseconds>(epoch_time).count();
        millisec_ = static_cast<int32_t>(millisec_since_epoch - (sec_since_epoch * 1000));
    }

    std::string logTimeString()
    {
        char datetime_sec[128];
        strftime(datetime_sec, sizeof(datetime_sec), "%Y-%m-%d %H:%M:%S", &now_);

        char datetime_millisec[128];
        snprintf(datetime_millisec, sizeof(datetime_millisec), "%s.%03d", datetime_sec, millisec_);
        return datetime_millisec;
    }

    std::string logFileNameString()
    {
        char datetime_sec[128];
        strftime(datetime_sec, sizeof(datetime_sec), "%Y%d%m_%H%M%S", &now_);
        return datetime_sec;
    }

  private:
    std::tm now_;
    int32_t millisec_;
};
} // namespace

namespace fw
{
class LogWriter::Impl
{
  public:
    Impl() : logpath_("./"), logname_("")
    {
        DateTime dt;
        logname_ = "log_" + dt.logFileNameString() + ".log";
    }

    void setLogPath(const std::string logpath)
    {
        logpath_ = logpath;
    }

    void write(const std::string level, const std::string tag, const std::string log)
    {
        DateTime dt;
        std::string datetime = dt.logTimeString();

        Result res = make_directory_recursive(logpath_);
        if (res == Result::NG)
        {
            printf("Failed to make directory -> %s\n", logpath_.c_str());
            return;
        }

        std::ofstream ofs;
        ofs.open(logpath_ + "/" + logname_, std::ios::app);
        ofs << datetime << " [" << level << "]"
            << "[" << tag << "] " << log << std::endl;
        ofs.close();
    }

  private:
    std::string logpath_;
    std::string logname_;
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
