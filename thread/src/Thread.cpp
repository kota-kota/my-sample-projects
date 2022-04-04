#include "Thread.hpp"
#include <sstream>

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

namespace
{

static std::uint64_t toIntergerFromThreadId(const std::thread::id &tid)
{
    std::uint64_t ret = 0;
    try
    {
        std::ostringstream os;
        os << tid;
        ret = std::stoul(os.str());
    }
    catch (...)
    {
        // std::stoulから、std::invalid_argument,std::out_of_rangeがthrowされる可能性あり
        ret = 0;
    }
    return ret;
}

} // namespace

namespace my
{

namespace thread
{

/// スレッドに名前を設定します
void setThreadName(std::thread &th, const std::string &name)
{
#ifdef WIN32
    size_t wcssize = 0;
    wchar_t wcstring[256];
    (void)::mbstowcs_s(&wcssize, wcstring, name.size() + 1, name.c_str(), _TRUNCATE);
    (void)::SetThreadDescription(th.native_handle(), wcstring);
#else
    pthread_setname_np(th.native_handle(), name.c_str());
#endif
}

/// スレッドの名前を取得します
std::string getThreadName(std::thread &th)
{
#ifdef WIN32
    char str[64] = {'\0'};
    PWSTR wstring;
    HRESULT result = ::GetThreadDescription(th.native_handle(), &wstring);
    if (SUCCEEDED(result))
    {
        size_t size = 0;
        (void)::wcstombs_s(&size, str, 64, wstring, 64);
        LocalFree(wstring);
    }
    return std::string(str);
#else
    char str[64] = {'\0'};
    pthread_getname_np(th.native_handle(), str, 64);
    return std::string(str);
#endif
}

/// スレッドIDを取得します
std::uint64_t getThreadId()
{
    return toIntergerFromThreadId(std::this_thread::get_id());
}
std::uint64_t getThreadId(const std::thread &th)
{
    return toIntergerFromThreadId(th.get_id());
}

} // namespace thread

} // namespace my
