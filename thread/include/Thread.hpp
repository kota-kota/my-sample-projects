#ifndef MY_THREAD_UTILITY_HPP
#define MY_THREAD_UTILITY_HPP

#include <string>
#include <thread>

namespace my
{

namespace thread
{

/// スレッドに名前を設定します
void setThreadName(std::thread &th, const std::string &name);

/// スレッドの名前を取得します
std::string getThreadName(std::thread &th);

/// スレッドIDを取得します
std::uint64_t getThreadId();
std::uint64_t getThreadId(const std::thread &th);

} // namespace thread

} // namespace my

#endif // MY_THREAD_UTILITY_HPP
