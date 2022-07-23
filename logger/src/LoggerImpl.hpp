#include "LogWriter.hpp"
#include <string>
#include <vector>

namespace fw
{
class LoggerImpl
{
  public:
    static LoggerImpl &getInstance();

  public:
    void setLogPath(const std::string logpath);
    void setFilteringTags(const std::vector<std::string> tags);
    void output(const std::string level, const std::string tag, const std::string log);

  private:
    LoggerImpl();

  private:
    LogWriter writer_;
    std::vector<std::string> tags_;
};
} // namespace fw
