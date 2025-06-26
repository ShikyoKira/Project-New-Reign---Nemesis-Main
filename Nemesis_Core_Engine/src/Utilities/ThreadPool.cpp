#include "Utilities/ThreadPool.h"

#include <iostream>


nemesis::ThreadPool::ThreadPool(size_t threads)
{
    abort = false;
    sync  = false;
    error = false;

    workers.reserve(threads);

    for (size_t i = 0; i < threads; ++i)
    {
        NewWorker();
    }
}

void nemesis::ThreadPool::join_all()
{
    sync = true;
    condition.notify_all();

    for (auto& worker : workers)
    {
        worker.join();
    }

    throw_if_error();
}

void nemesis::ThreadPool::stop()
{
    abort = true;
    condition.notify_all();
}

bool nemesis::ThreadPool::has_error() const noexcept
{
    return error;
}

void nemesis::ThreadPool::throw_if_error()
{
    if (!error) return;

    throw nemesis::ThreadPool::ThreadException();
}

nemesis::ThreadPool::~ThreadPool()
{
    stop();

    if (!sync) join_all();
}

void nemesis::ThreadPool::NewWorker()
{
    workers.emplace_back([&] {
        for (;;)
        {
            ThreadPoolTask task;

            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                condition.wait(lock, [&] { return error || abort || sync || !tasks.empty(); });

                if (error || abort || tasks.empty()) return;

                task = std::move(tasks.top());
                tasks.pop();
            }

            task.task();
        }
    });
}
