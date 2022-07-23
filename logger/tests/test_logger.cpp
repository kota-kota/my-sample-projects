
#include "Logger.hpp"

void test_logger()
{
    fw::LoggerOptions options;
    options.withLogPath("./logs").withFilteringTags({"NO_OUTPUT", "VERBOSE"}).apply();
    fw::Logger::error("TAG1", "hoge");
    fw::Logger::error("TAG2", "hogehoge");
    fw::Logger::error("TAG3", "hogehogehoge");
    fw::Logger::error("VERBOSE", "no hogehogehoge");
    fw::Logger::error("TAG4", "Name: %s Age: %d", "Sato Taro", 29);
    fw::Logger::error("TAG5", "名前: %s 年齢: %d", "佐藤太郎", 29);
    fw::Logger::error("NO_OUTPUT", "no output log");
}
