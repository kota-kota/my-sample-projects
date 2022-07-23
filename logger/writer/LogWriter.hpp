#include <string>
#include <vector>

namespace fw
{
class LogWriter
{
  public:
    LogWriter();
    ~LogWriter();
    void setLogPath(const std::string logpath);
    void write(const std::string level, const std::string tag, const std::string log);

  private:
    class Impl;
    Impl *impl_;
    std::string logpath_;
};
} // namespace fw
