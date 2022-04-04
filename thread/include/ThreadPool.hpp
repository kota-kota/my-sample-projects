#include "Queue.hpp"
#include "Thread.hpp"
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>

namespace my
{
class ThreadPool
{
  private:
    Queue<std::function<void()>> queue_;
    std::vector<std::thread> threads_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool isRunning_;

  public:
    ThreadPool(const int32_t threadCount, const int32_t queueSize, const std::string name = "")
        : queue_(queueSize), threads_(), mutex_(), cv_(), isRunning_(true)
    {
        for (int32_t i = 0; i < threadCount; i++)
        {
            threads_.emplace_back(std::thread(main_));
            if (!name.empty())
            {
                thread::setThreadName(threads_.at(i), name + std::to_string(i));
            }
        }
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            isRunning_ = false;
        }
        cv_.notify_all();
        const int32_t size = static_cast<int32_t>(threads_.size());
        for (int32_t i = 0; i < size; i++)
        {
            threads_.at(i).join();
        }
    }

    bool add(std::function<void()> &&func)
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            const bool result = queue_.enqueue(std::move(func));
            if (!result)
            {
                return false;
            }
            cv_.notify_all();
            return true;
        }
    }

    bool add(const std::function<void()> &func)
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            const bool result = queue_.enqueue(func);
            if (!result)
            {
                return false;
            }
            cv_.notify_all();
            return true;
        }
    }

  private:
    std::function<void()> main_ = [this]() {
        while (true)
        {
            std::function<void()> func;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                while (queue_.empty())
                {
                    if (!isRunning_)
                    {
                        return;
                    }
                    cv_.wait(lock);
                }
                (void)queue_.dequeue(func);
            }
            func();
        }
    };
};
} // namespace my
