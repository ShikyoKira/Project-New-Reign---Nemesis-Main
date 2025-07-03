#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>
#include <queue>
#include <iostream>

#include "Logger.h"

extern std::atomic_bool StopProcessFlag;

namespace nemesis
{
    struct ThreadPool
    {
        struct ThreadException : std::exception
        {
        };

    private:
        struct ThreadPoolTask
        {
            long priority;
            std::function<void()> task;

            inline ThreadPoolTask() = default;
            inline ThreadPoolTask(long p, std::function<void()> t)
            {
                priority = p;
                task     = t;
            }

            inline bool operator<(const ThreadPoolTask& other) const
            {
                return priority < other.priority;
            }
        };

    public:
        ThreadPool(size_t threads = std::thread::hardware_concurrency());

        template <class F, class... Args>
        decltype(auto) enqueue(F&& f, Args&&... args);

        template <class F, class... Args>
        decltype(auto) priority_enqueue(long priority, F&& f, Args&&... args);

        void join_all();
        void stop();
        bool has_error() const noexcept;
        void throw_if_error();

        ~ThreadPool();
    private:
        std::vector<std::thread> workers;
        std::priority_queue<ThreadPoolTask> tasks;
        void NewWorker();

        std::condition_variable condition;
        std::mutex queue_mutex;
        std::atomic<bool> abort;
        std::atomic<bool> sync;
        std::atomic<bool> error;
    };

    // add new work item to the pool
    template <class F, class... Args>
    decltype(auto) ThreadPool::enqueue(F&& f, Args&&... args)
    {
        return priority_enqueue(1, std::forward<F>(f), std::forward<Args>(args)...);
    }

    // add new work item to the pool
    template <class F, class... Args>
    decltype(auto) ThreadPool::priority_enqueue(long priority, F&& f, Args&&... args)
    {
        using return_type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> future = task->get_future();
        std::shared_future<return_type> shared_future = future.share(); 

        {
            if (StopProcessFlag) return shared_future;

            if (error) return shared_future;

            // don't allow enqueueing after stopping the pool
            if (abort) throw std::runtime_error("Failed to enqueue on stopped ThreadPool");

            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(priority,
                          [task, shared_future, this]
                          {
                              (*task)();

                              try
                              {
                                  shared_future.get();
                              }
                              catch (const std::exception& ex)
                              {
                                  StopProcessFlag = true;
                                  Logger::Log(std::string("[ERROR] ") + ex.what(), true);
                                  error = true;
                              }
                              catch (...)
                              {
                                  StopProcessFlag = true;
                                  Logger::Log("[ERROR] Unknown exception captured", true);
                                  error = true;
                              }
                          });
        }

        condition.notify_one();
        return shared_future;
    }
}

#endif
