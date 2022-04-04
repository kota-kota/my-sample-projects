#include "LockedQueue.hpp"
#include "Queue.hpp"
#include <chrono>
#include <cstdio>
#include <thread>

class Data
{
  private:
    std::int32_t m_value_ = 0;

  public:
    // コンストラクタ
    Data()
    {
    }
    explicit Data(std::int32_t value)
    {
        m_value_ = value;
    }
    // デストラクタ
    ~Data()
    {
    }
    // コピーコンストラクタ
    Data(const Data &org)
    {
        printf("  Data::Copy Constructor\n");
        *this = org;
    }
    // コピー代入演算子
    Data &operator=(const Data &org)
    {
        printf("  Data::Copy Operator\n");
        if (this != &org)
        {
            m_value_ = org.m_value_;
        }
        return *this;
    }
    // ムーブコンストラクタ
    Data(Data &&org)
    {
        printf("  Data::Move Constructor\n");
        *this = std::move(org);
    }
    // ムーブ代入演算子
    Data &operator=(Data &&org)
    {
        printf("  Data::Move Operator\n");
        if (this != &org)
        {
            m_value_ = org.m_value_;
        }
        return *this;
    }

    bool operator==(const int org)
    {
        if (m_value_ == org)
        {
            return true;
        }
        return false;
    }

    operator int() const
    {
        return m_value_;
    }
};

class Hoge
{
    std::thread th_;
    my::LockedQueue<Data> que_;

  public:
    Hoge(std::size_t capacity) : que_(capacity)
    {
        std::thread th([this]() {
            printf("  task start\n");
            while (true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                Data data;
                que_.dequeue(data);
                printf("  task deq:%d\n", static_cast<std::int32_t>(data));
                if (data == -1)
                {
                    break;
                }
            }
            printf("  task end\n");
        });
        th_.swap(th);
    }

    ~Hoge()
    {
        th_.join();
    }

    void set(Data &&data)
    {
        printf("  task enq:%d\n", static_cast<std::int32_t>(data));
        que_.enqueue(data);
    }
};

void test_queue()
{
    printf("<< test_queue >>\n");
    {
        printf("* LockedQueue\n");
        Hoge h(4);
        for (std::int32_t i = 0; i < 10; i++)
        {
            h.set(Data(i));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("  FINISH\n");
        h.set(Data(-1));
    }
    {
        printf("* Queue\n");
        my::Queue<Data> que(3);
        for (std::int32_t i = 0; i < 5; i++)
        {
            bool res = que.enqueue(Data(i));
            printf("  enq:%d %d\n", i, res);
        }
        for (std::int32_t i = 0; i < 5; i++)
        {
            Data data;
            bool res = que.dequeue(data);
            printf("  deq:%d %d\n", static_cast<std::int32_t>(data), res);
        }
        for (std::int32_t i = 0; i < 5; i++)
        {
            Data data(i);
            bool res = que.enqueue(data);
            printf("  enq:%d %d\n", i, res);
        }
        for (std::int32_t i = 0; i < 5; i++)
        {
            Data data;
            bool res = que.dequeue(data);
            printf("  deq:%d %d\n", static_cast<std::int32_t>(data), res);
        }
    }
}
