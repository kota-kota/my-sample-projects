
#include "function.hpp"
#include <functional>

void test_memory()
{
    printf("<< test_memory >>\n");

    std::function<void(std::string, std::int16_t *, std::int32_t)> array_print =
        [](std::string tag, std::int16_t *array, std::int32_t num) {
            printf("  %s: ", tag.c_str());
            for (std::int32_t i = 0; i < num; i++)
            {
                printf("%d ", array[i]);
            }
            printf("\n");
        };

    {
        constexpr std::int32_t MAX_NUM = 4;
        constexpr std::int32_t MAX_SIZE = MAX_NUM * sizeof(std::int16_t);
        printf("* memset\n");
        std::int16_t buf[MAX_NUM];
        fw::memset(buf, 0, MAX_SIZE);
        array_print("buf", buf, MAX_NUM);
    }
    {
        constexpr std::int32_t MAX_NUM = 4;
        constexpr std::int32_t MAX_SIZE = MAX_NUM * sizeof(std::int16_t);
        printf("* memcpy\n");
        std::int16_t src[MAX_NUM] = {1, 2, 3, 4};
        std::int16_t dest[MAX_NUM] = {5, 6, 7, 8};
        array_print("src", src, MAX_NUM);
        array_print("dest before", dest, MAX_NUM);
        fw::memcpy(dest, MAX_SIZE, src, MAX_SIZE);
        array_print("dest after", dest, MAX_NUM);
    }
    {
        constexpr std::int32_t MAX_NUM = 4;
        constexpr std::int32_t MAX_SIZE = MAX_NUM * sizeof(std::int16_t);
        constexpr std::int32_t DEST_MAX_NUM = 8;
        constexpr std::int32_t DEST_MAX_SIZE = DEST_MAX_NUM * sizeof(std::int16_t);
        printf("* memcpy\n");
        std::int16_t src[MAX_NUM] = {1, 2, 3, 4};
        std::int16_t dest[DEST_MAX_NUM] = {5, 6, 7, 8, 9, 10, 11, 12};
        array_print("src", src, MAX_NUM);
        array_print("dest before", dest, DEST_MAX_NUM);
        fw::memcpy(dest, DEST_MAX_SIZE, src, MAX_SIZE);
        array_print("dest after", dest, DEST_MAX_NUM);
    }
    {
        constexpr std::int32_t MAX_NUM = 4;
        constexpr std::int32_t MAX_SIZE = MAX_NUM * sizeof(std::int16_t);
        constexpr std::int32_t DEST_MAX_NUM = 2;
        constexpr std::int32_t DEST_MAX_SIZE = DEST_MAX_NUM * sizeof(std::int16_t);
        printf("* memcpy\n");
        std::int16_t src[MAX_NUM] = {1, 2, 3, 4};
        std::int16_t dest[DEST_MAX_NUM] = {5, 6};
        array_print("src", src, MAX_NUM);
        array_print("dest before", dest, DEST_MAX_NUM);
        fw::memcpy(dest, DEST_MAX_SIZE, src, MAX_SIZE);
        array_print("dest after", dest, DEST_MAX_NUM);
    }
}

void test_env()
{
    printf("<< test_env >>\n");
    {
        printf("* getenv exist\n");
        std::string var = fw::getenv("PATH");
        printf("  PATH: %s\n", var.c_str());
    }
    {
        printf("* getenv no exist\n");
        std::string var = fw::getenv("NO_VAR");
        printf("  NO_VAR: %s\n", var.c_str());
    }
}

void test_str()
{
    printf("<< test_str >>\n");
    {
        printf("* strlen\n");
        std::string str = "Hello,world!";
        std::int32_t len = (std::int32_t)fw::strlen(str.c_str());
        printf("  str: %s\n", str.c_str());
        printf("  len: %d\n", len);
    }
    {
        printf("* strnlen\n");
        std::string str = "Hello,world!";
        std::int32_t len = (std::int32_t)fw::strnlen(str.c_str(), 5);
        printf("  str: %s\n", str.c_str());
        printf("  len: %d\n", len);
    }
    {
        printf("* strcpy\n");
        std::string src = "Hello,world!";
        char *dest = new char[100];
        fw::strcpy(dest, 100, src.c_str());
        printf("  src: %s\n", src.c_str());
        printf("  dest: %s\n", dest);
        delete[] dest;
    }
    {
        printf("* strncpy\n");
        std::string src = "Hello,world!";
        char *dest = new char[100];
        fw::strncpy(dest, 100, src.c_str(), 5);
        printf("  src: %s\n", src.c_str());
        printf("  dest: %s\n", dest);
        delete[] dest;
    }
}

void test_function()
{
    test_memory();
    test_env();
    test_str();
}
