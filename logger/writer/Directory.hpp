#include <string>

namespace fw
{
enum class Result
{
    NG = -1,
    OK,
    EXIST,
};
Result make_directory(const std::string dirpath);
Result make_directory_recursive(const std::string dirpath);
} // namespace fw