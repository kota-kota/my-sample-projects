#include "function.hpp"
#include <cstdlib>
#include <cstring>

namespace fw
{
std::size_t strlen(const char *str)
{
    return std::strlen(str);
}
std::size_t strnlen(const char *str, std::size_t maxlen)
{
#if WIN32
    return strnlen_s(str, maxlen);
#else
    return ::strnlen(str, maxlen);
#endif
}
void strcpy(char *dest, std::size_t destSize, const char *src)
{
#if WIN32
    strcpy_s(dest, destSize, src);
#else
    if (destSize <= 0)
    {
        return;
    }
    std::strcpy(dest, src);
#endif
}
void strncpy(char *dest, std::size_t destSize, const char *src, std::size_t count)
{
#if WIN32
    strncpy_s(dest, destSize, src, count);
#else
    if (destSize <= 0)
    {
        return;
    }
    std::strncpy(dest, src, count);
#endif
}
} // namespace fw

namespace fw
{
std::string getenv(const std::string varname)
{
#if WIN32
    std::size_t requiredSize = 0;
    (void)getenv_s(&requiredSize, nullptr, 0, varname.c_str());
    if (requiredSize <= 0)
    {
        return "";
    }
    char *tmpvar = new (std::nothrow) char[requiredSize];
    if (tmpvar == nullptr)
    {
        return "";
    }
    (void)getenv_s(&requiredSize, tmpvar, requiredSize, varname.c_str());
    std::string retvar = tmpvar;
    delete[] tmpvar;
    return retvar;
#else
    char *tmpvar = std::getenv(varname.c_str());
    if (tmpvar == nullptr)
    {
        return "";
    }
    return tmpvar;
#endif
}
} // namespace fw

namespace fw
{
void fopen(FILE **fp, const char *filename, const char *modes)
{
#if WIN32
    (void)fopen_s(fp, filename, modes);
#else
    *fp = std::fopen(filename, modes);
#endif
}

void fread(void *buffer, std::size_t bufferSize, std::size_t elementSize, std::size_t elementCount, FILE *fp)
{
#if WIN32
    (void)fread_s(buffer, bufferSize, elementSize, elementCount, fp);
#else
    if (bufferSize < (elementSize * elementCount))
    {
        return;
    }
    (void)std::fread(buffer, elementSize, elementCount, fp);
#endif
}
} // namespace fw

namespace fw
{
void memset(void *dest, std::int32_t val, std::size_t count)
{
    (void)std::memset(dest, val, count);
}

void memcpy(void *dest, std::size_t destSize, void *src, std::size_t srcSize)
{
#if WIN32
    if (destSize < srcSize)
    {
        return;
    }
    (void)memcpy_s(dest, destSize, src, srcSize);
#else
    if (destSize < srcSize)
    {
        return;
    }
    (void)std::memcpy(dest, src, srcSize);
#endif
}
} // namespace fw
