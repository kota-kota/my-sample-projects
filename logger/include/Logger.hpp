#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace fw
{
class LoggerOptions
{
  public:
    LoggerOptions();
    LoggerOptions &withLogPath(const std::string logpath);
    LoggerOptions &withFilteringTags(const std::vector<std::string> tags);
    void apply();

  private:
    std::string logpath_;
    std::vector<std::string> tags_;
};

class Logger
{
  public:
    template <typename... Args>
    static void error(const std::string tag, const std::string format, Args const &...args)
    {
        output("ERROR", tag, format.c_str(), args...);
    }

    template <typename... Args>
    static void info(const std::string tag, const std::string format, Args const &...args)
    {
        output("INFO", tag, format.c_str(), args...);
    }

  private:
    static void output(const std::string level, const std::string tag, const char *const format, ...);
};
} // namespace fw
