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

namespace nemesis
{
    using uint = unsigned int;

    class ThreadPool
    {
    public:
        ThreadPool(uint threads = std::thread::hardware_concurrency());

        template <class F, class... Args>
        decltype(auto) enqueue(F&& f, Args&&... args);

        void join_all();
        void stop();

        ~ThreadPool();

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        void newWorker();

        std::condition_variable condition;
        std::mutex queue_mutex;
        bool abort = false;
        bool sync = false;
    };

    // add new work item to the pool
    template <class F, class... Args>
    decltype(auto) ThreadPool::enqueue(F&& f, Args&&... args)
    {
        using return_type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            if (error) return res;

            // don't allow enqueueing after stopping the pool
            if (abort) throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });
        }

        condition.notify_one();
        return res;
    }
}

#endif
