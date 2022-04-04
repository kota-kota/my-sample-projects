#include "Timer.hpp"
#include "Thread.hpp"

namespace my
{

Timer::Timer() : th_(), isRunning_(false), interval_(0), name_("Worker")
{
}

Timer::~Timer()
{
    stop();
}

void Timer::setName(const std::string &name)
{
    name_ = name;
}

void Timer::setInterval(std::chrono::milliseconds interval)
{
    interval_ = interval;
}

void Timer::start(std::function<void()> func)
{
    if (isRunning_)
    {
        return;
    }

    isRunning_ = true;
    std::thread th([this, func]() {
        printf("  [0x%zu] Start timer thread (%s)\n", my::thread::getThreadId(), name_.c_str());
        std::int64_t adjust_time = 0;
        while (isRunning_)
        {
            std::int64_t wait_time = interval_.count() - adjust_time;
            if (wait_time < 0)
            {
                wait_time = interval_.count();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
            if (!isRunning_)
            {
                break;
            }

            std::chrono::system_clock::time_point sta = std::chrono::system_clock::now();
            func();
            std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
            adjust_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - sta).count();
        }
        printf("  [0x%zu] End timer thread (%s)\n", my::thread::getThreadId(), name_.c_str());
    });
    my::thread::setThreadName(th, name_);
    th.swap(th_);
}

void Timer::stop()
{
    if (!isRunning_)
    {
        return;
    }
    isRunning_ = false;
    th_.join();
}

void Timer::timeout(std::chrono::milliseconds interval, std::function<void()> func)
{
    std::thread th([interval, func]() {
        printf("  [0x%zu] Start timeout thread\n", my::thread::getThreadId());
        std::this_thread::sleep_for(interval);
        func();
        printf("  [0x%zu] End timeout thread\n", my::thread::getThreadId());
    });
    my::thread::setThreadName(th, "TimerTimeOut");
    th.detach();
}
} // namespace my
