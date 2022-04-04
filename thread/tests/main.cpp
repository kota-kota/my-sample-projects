#include <cstdio>

void test_thread();
void test_threadpool();
void test_timer();
void test_queue();
void sample_async();
int main()
{
    printf("main start\n");
    test_thread();
    test_timer();
    test_queue();
    test_threadpool();
    sample_async();
    printf("main end\n");
    return 0;
}
