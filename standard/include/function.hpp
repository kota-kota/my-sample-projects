#include <cstdint>
#include <cstdio>
#include <string>

namespace fw
{
std::size_t strlen(const char *str);
std::size_t strnlen(const char *str, std::size_t maxlen);
void strcpy(char *dest, std::size_t destSize, const char *src);
void strncpy(char *dest, std::size_t destSize, const char *src, std::size_t count);
} // namespace fw

namespace fw
{
std::string getenv(const std::string varname);
} // namespace fw

namespace fw
{
void fopen(FILE **fp, const char *filename, const char *modes);
void fread(void *buffer, std::size_t bufferSize, std::size_t elementSize, std::size_t elementCount, FILE *fp);
} // namespace fw

namespace fw
{
void memset(void *dest, std::int32_t val, std::size_t count);
void memcpy(void *dest, std::size_t destSize, void *src, std::size_t srcSize);
} // namespace fw
