#include "Thread.hpp"
#include "Timer.hpp"

namespace
{
static void PrintElapsedTime(std::chrono::system_clock::time_point before, std::chrono::system_clock::time_point after)
{
    std::chrono::microseconds before_msec =
        std::chrono::duration_cast<std::chrono::microseconds>(before.time_since_epoch());
    std::chrono::microseconds after_msec =
        std::chrono::duration_cast<std::chrono::microseconds>(after.time_since_epoch());
    double elapsed = (after_msec.count() - before_msec.count()) / 1000.0;
    printf("  [0x%zu] Interval: %.3f(msec)\n", my::thread::getThreadId(), elapsed);
}
} // namespace

// 一定時間間隔でタイマーを開始するテスト
// 通常の関数を使用するケース
class TimerTest_Interval_Function
{
    my::Timer timer_;
    static std::chrono::system_clock::time_point before_;

  private:
    static void func()
    {
        std::chrono::system_clock::time_point after = std::chrono::system_clock::now();
        PrintElapsedTime(before_, after);
        before_ = after;
    }

  public:
    void run()
    {
        before_ = std::chrono::system_clock::now();
        timer_.setInterval(std::chrono::milliseconds(600));
        timer_.setName("TimerThread");
        timer_.start(TimerTest_Interval_Function::func);
    }
    void stop()
    {
        timer_.stop();
    }
};
std::chrono::system_clock::time_point TimerTest_Interval_Function::before_ = std::chrono::system_clock::now();

// 一定時間間隔でタイマーを開始するテスト
// ラムダ式を使用するケース
class TimerTest_Interval_Lamda
{
    my::Timer timer_;
    std::chrono::system_clock::time_point before_{std::chrono::system_clock::now()};

  public:
    void run()
    {
        timer_.setInterval(std::chrono::milliseconds(600));
        timer_.setName("TimerThread");
        timer_.start([this]() {
            std::chrono::system_clock::time_point after = std::chrono::system_clock::now();
            PrintElapsedTime(before_, after);
            before_ = after;
        });
    }
    void stop()
    {
        timer_.stop();
    }
};

// 一定時間間隔でタイマーを開始するテスト
// メンバ関数を使用するケース
class TimerTest_Interval_ClassFunction
{
    my::Timer timer_;
    std::chrono::system_clock::time_point before_{std::chrono::system_clock::now()};

  public:
    void func()
    {
        std::chrono::system_clock::time_point after = std::chrono::system_clock::now();
        PrintElapsedTime(before_, after);
        before_ = after;
    }

  public:
    void run()
    {
        timer_.setInterval(std::chrono::milliseconds(600));
        timer_.setName("TimerThread");
        std::function<void(TimerTest_Interval_ClassFunction *)> f = &TimerTest_Interval_ClassFunction::func;
        timer_.start(std::bind(f, this));
    }
    void stop()
    {
        timer_.stop();
    }
};

// 指定時間後に一度だけコールするテスト
class TimerTest_TimeOut
{
    std::chrono::system_clock::time_point before_{std::chrono::system_clock::now()};

  public:
    void run()
    {
        my::Timer::timeout(std::chrono::milliseconds(1000),
                           [=]() { PrintElapsedTime(before_, std::chrono::system_clock::now()); });
    }
};

void test_timer()
{
    printf("<< test_timer >>\n");
    {
        printf("* TimerTest_Interval_Function\n");
        TimerTest_Interval_Function test;
        test.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        test.stop();
    }
    {
        printf("* TimerTest_Interval_Lamda\n");
        TimerTest_Interval_Lamda test;
        test.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        test.stop();
    }
    {
        printf("* TimerTest_Interval_ClassFunction\n");
        TimerTest_Interval_ClassFunction test;
        test.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        test.stop();
    }
    {
        printf("* TimerTest_TimeOut\n");
        TimerTest_TimeOut test;
        test.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}