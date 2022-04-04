#ifndef MY_TIMER_HPP
#define MY_TIMER_HPP

#include <atomic>
#include <chrono>
#include <functional>
#include <string>
#include <thread>

namespace my
{

class Timer final
{
    std::thread th_;
    std::atomic<bool> isRunning_;
    std::chrono::milliseconds interval_;
    std::string name_;

  public:
    Timer(const Timer &other) = delete;
    Timer &operator=(const Timer &other) = delete;

  public:
    /// タイマーオブジェクトを生成します
    Timer();
    /// タイマーオブジェクトを破棄します
    ~Timer();
    /// タイマースレッドに名前を付けます
    void setName(const std::string &name);
    /// タイマーの時間間隔を msec ミリ秒に設定します
    /// タイマー開始後に変更することもできます
    void setInterval(std::chrono::milliseconds interval);
    /// setInterval()で設定された時間間隔で func を呼び出すタイマーを開始します
    /// タイマーが既に開始されている場合は何もしません
    void start(std::function<void()> func);
    /// タイマーを停止します
    void stop();

  public:
    /// interval ミリ秒後に、一度だけ func を呼び出します
    static void timeout(std::chrono::milliseconds interval, std::function<void()> func);
};

} // namespace my

#endif // MY_TIMER_HPP
