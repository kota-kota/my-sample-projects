#include <climits>
#include <condition_variable>
#include <deque>
#include <mutex>

namespace my
{
template <class T>
class LockedQueue
{
    std::size_t capacity_;
    std::deque<T> que_;
    std::mutex mtx_;
    std::condition_variable c_enq_;
    std::condition_variable c_deq_;

  public:
    LockedQueue()
    {
        capacity_ = USHRT_MAX;
    }

    explicit LockedQueue(std::size_t capacity)
    {
        capacity_ = capacity;
    }

    void enqueue(const T &data)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        // 外からnotify_all()またはnotify_one()によって起こされるまでブロックして待つ
        // ただし、起こされた時にキューが満杯だった場合は、またブロックして待つ
        c_enq_.wait(lock, [this] { return que_.size() != capacity_; });
        que_.emplace_back(data);
        // dequeueの準備ができたことを通知
        c_deq_.notify_one();
    }

    void enqueue(T &&data)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        // 外からnotify_all()またはnotify_one()によって起こされるまでブロックして待つ
        // ただし、起こされた時にキューが満杯だった場合は、またブロックして待つ
        c_enq_.wait(lock, [this] { return que_.size() != capacity_; });
        que_.emplace_back(std::move(data));
        // dequeueの準備ができたことを通知
        c_deq_.notify_one();
    }

    // キューから要素を取り出す
    void dequeue(T &data)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        // 外からnotify_all()またはnotify_one()によって起こされるまでブロックして待つ
        // ただし、起こされた時にキューが空である場合は、またブロックして待つ
        c_deq_.wait(lock, [this] { return !que_.empty(); });
        data = std::move(que_.front());
        que_.pop_front();
        // enqueueの準備ができたことを通知
        c_enq_.notify_one();
    }
};
} // namespace my
