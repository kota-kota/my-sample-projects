#include "Thread.hpp"
#include <cstdio>

class Hoge
{
    std::thread th_;

  public:
    Hoge(const std::string name)
    {
        std::thread th([name]() {
            std::uint64_t tid = my::thread::getThreadId();
            printf("  [%s] tid:0x%zu\n", name.c_str(), tid);
        });
        my::thread::setThreadName(th, name);
        std::uint64_t tid = my::thread::getThreadId(th);
        std::string tname = my::thread::getThreadName(th);
        printf("  [%s] tid:0x%zu tname:%s\n", name.c_str(), tid, tname.c_str());
        th_.swap(th);
    }
    ~Hoge()
    {
        th_.join();
    }
};

void test_thread()
{
    printf("<< test_thread >>\n");
    Hoge h1("Hoge01");
    Hoge h2("Hoge02");
    Hoge h3("Hoge03");
}
