#include "ThreadPool.hpp"
#include <cstdio>

void test_threadpool()
{
    printf("<< test_threadpool >>\n");
    {
        my::ThreadPool tp(2, 5, "ThreadPool");

        std::function<void(const std::int32_t)> worker = [](const std::int32_t no) {
            printf("  work! No.%d\n", no);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        };

        std::int32_t next = 0;
        for (std::int32_t i = 0; i < 10; i++)
        {
            bool res = tp.add(std::bind(worker, i));
            next++;
            printf("  add No.%d (%d)\n", i, res);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        for (std::int32_t i = 0; i < 5; i++)
        {
            bool res = tp.add(std::bind(worker, i + next));
            printf("  add No.%d (%d)\n", i + next, res);
        }
    }
}
