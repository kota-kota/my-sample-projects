#include "Directory.hpp"
#include <cstdio>

#include <algorithm>
#ifdef WIN32
#include <direct.h>
#endif
#include <istream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

namespace
{
#ifdef WIN32
constexpr char PATH_SEP = '\\';
#else
constexpr char PATH_SEP = '/';
#endif

std::vector<std::string> split(const std::string s, const char separator)
{
    std::vector<std::string> v;
    std::stringstream ss{s};
    std::string buf;
    while (std::getline(ss, buf, separator))
    {
        v.push_back(buf);
    }
    return v;
}
} // namespace

namespace fw
{
Result make_directory(const std::string dirpath)
{
    // ディレクトリ存在チェック
    {
        struct stat statBuf;
        if (stat(dirpath.c_str(), &statBuf) == 0)
        {
            // 既に存在しているので成功で返す
            // printf("Exist directory [%s]\n", dirpath.c_str());
            return Result::EXIST;
        }
    }

    // ディレクトリ作成
    Result res = Result::OK;
#ifdef WIN32
    if (_mkdir(dirpath.c_str()) == 0)
#else
    if (mkdir(dirpath.c_str(), 0755) == 0)
#endif
    {
        // printf("Success to make directory [%s]\n", dirpath.c_str());
        res = Result::OK;
    }
    else
    {
        // printf("Failed to make directory [%s]\n", dirpath.c_str());
        res = Result::NG;
    }
    return res;
}
Result make_directory_recursive(const std::string dirpath)
{
    std::string s = dirpath;
#ifdef WIN32
    std::replace(s.begin(), s.end(), '/', '\\');
#endif
    // printf("Replace %s -> %s\n", dirpath.c_str(), s.c_str());

    Result res = Result::OK;
    std::string make_dir;
    std::vector<std::string> vs = split(s, PATH_SEP);
    for (std::string dir : vs)
    {
        make_dir += dir + PATH_SEP;
        res = make_directory(make_dir);
    }
    return res;
}
} // namespace fw
