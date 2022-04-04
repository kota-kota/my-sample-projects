#include <atomic>
#include <chrono>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <string>
#include <thread>

class Server
{
  public:
    static std::string service(std::string opt)
    {
        constexpr std::int32_t MAX_COUNT = 10;
        static std::int32_t counter = 0;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (counter == MAX_COUNT)
        {
            return "";
        }
        counter++;

        std::string str;
        for (std::int32_t i = 0; i < counter; i++)
        {
            str += opt;
        }
        return str;
    }

  public:
    std::future<std::string> request(std::string opt)
    {
        std::packaged_task<std::string(std::string)> task(Server::service);

        std::future<std::string> result = task.get_future();

        std::thread th(std::move(task), opt);
        th.detach();

        return result;
    }
};

class Client
{
  private:
    Server m_server_;
    std::function<void(std::string)> m_callback_;
    std::thread m_th_;
    std::atomic_bool m_running_;

  private:
    void onMessage(std::string msg)
    {
        std::thread th([this, msg]() { m_callback_(msg); });
        th.detach();
    }

  public:
    ~Client()
    {
        end();
    }

    void addOnMessage(std::function<void(std::string)> callback)
    {
        m_callback_ = callback;
    }

    void start()
    {
        m_running_ = true;
        std::thread th([this]() {
            while (m_running_)
            {
                std::future<std::string> result = m_server_.request("^");
                onMessage(result.get());
            }
        });
        m_th_.swap(th);
    }

    void end()
    {
        m_running_ = false;
        if (m_th_.joinable())
        {
            m_th_.join();
        }
    }
};

class Application
{
  private:
    Client m_client_;
    std::string m_msg_;
    std::mutex m_mtx_;
    bool m_update_ = false;
    bool m_finish_ = false;

  public:
    void start()
    {
        m_client_.addOnMessage([this](std::string msg) {
            std::lock_guard<std::mutex> lock(m_mtx_);
            if (msg.empty())
            {
                m_finish_ = true;
            }
            else
            {
                m_update_ = true;
                m_msg_ = msg;
            }
        });
        m_client_.start();
    }

    void end()
    {
        m_client_.end();
    }

    void display()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::lock_guard<std::mutex> lock(m_mtx_);
            if (m_update_)
            {
                printf("  %s\n", m_msg_.c_str());
                m_update_ = false;
            }
            if (m_finish_)
            {
                printf("  finish\n");
                break;
            }
        }
    }
};

void sample_async()
{
    printf("<< sample_async >>\n");
    Application app;
    app.start();
    app.display();
    app.end();
}