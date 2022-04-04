#include <cstdio>

void sample_rvalue();
void test_function();
int main()
{
    printf("main start\n");
    sample_rvalue();
    test_function();
    printf("main end\n");
    return 0;
}