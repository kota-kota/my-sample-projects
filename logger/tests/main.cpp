#include <cstdio>
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

void test_directory()
{
    fw::Result ok;
    std::string dir;
    {
        ok = fw::make_directory(dir = "dir1");
        printf("%d -> %s\n", ok, dir.c_str());
    }
    {
        ok = fw::make_directory(dir = "./dir2/");
        printf("%d -> %s\n", ok, dir.c_str());
    }
    {
        ok = fw::make_directory(dir = "./dir3/r1/r2");
        printf("%d -> %s\n", ok, dir.c_str());
    }
    {
#ifdef WIN32
        ok = fw::make_directory(dir = "C:\\");
#else
        ok = fw::make_directory(dir = "/usr/dir");
#endif
        printf("%d -> %s\n", ok, dir.c_str());
    }
    {
#ifdef WIN32
        ok = fw::make_directory(dir = "C:\\workspace\\my-sample-projects\\build_windows\\logger\\tests\\dir4");
#else
        ok = fw::make_directory(dir = "/workspaces/my-sample-projects/build_clang/logger/tests/dir4");
#endif
        printf("%d -> %s\n", ok, dir.c_str());
    }
    {
        ok = fw::make_directory_recursive(dir = "dir5");
        printf("%d -> %s\n", ok, dir.c_str());
    }
    {
        ok = fw::make_directory_recursive(dir = "./dir6/");
        printf("%d -> %s\n", ok, dir.c_str());
    }
    {
        ok = fw::make_directory_recursive(dir = "./dir7/r1/r2");
        printf("%d -> %s\n", ok, dir.c_str());
    }
}

void test_logger();
int main()
{
    printf("main start\n");
    test_logger();
    test_directory();
    printf("main end\n");
    return 0;
}