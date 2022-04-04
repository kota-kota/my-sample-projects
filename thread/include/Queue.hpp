
#include <climits>
#include <deque>
#include <mutex>

namespace my
{
template <typename T>
class Queue
{
  private:
    std::size_t capacity_;
    std::deque<T> deque_;

  public:
    Queue()
    {
        capacity_ = USHRT_MAX;
    }

    explicit Queue(std::size_t capacity)
    {
        capacity_ = capacity;
    }

    bool enqueue(T &&data)
    {
        if (capacity_ <= deque_.size())
        {
            return false;
        }
        deque_.emplace_back(std::move(data));
        return true;
    }

    bool enqueue(const T &data)
    {
        if (capacity_ <= deque_.size())
        {
            return false;
        }
        deque_.emplace_back(data);
        return true;
    }

    bool dequeue(T &data)
    {
        if (deque_.empty())
        {
            return false;
        }
        data = std::move(deque_.front());
        deque_.pop_front();
        return true;
    }

    bool empty() const
    {
        return deque_.empty();
    }
};
} // namespace my
