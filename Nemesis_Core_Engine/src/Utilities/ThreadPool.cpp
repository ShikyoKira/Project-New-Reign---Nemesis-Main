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

void nemesis::ThreadPool::wait_for_all()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    condition.wait(lock, [&] { return tasks.empty(); });
    throw_if_error();
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
    return error || StopProcessFlag;
}

void nemesis::ThreadPool::throw_if_error()
{
    if (!error && !StopProcessFlag) return;

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
            bool is_empty;

            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                condition.wait(lock, [&] { return StopProcessFlag || error || abort || sync || !tasks.empty(); });

                if (StopProcessFlag || error || abort) return;

                if (sync && tasks.empty()) return;

                if (tasks.empty()) continue;

                task = std::move(tasks.top());
                tasks.pop();
                is_empty = tasks.empty();
            }

            task.task();

            if (!is_empty) continue;

            condition.notify_all();
        }
    });
}
