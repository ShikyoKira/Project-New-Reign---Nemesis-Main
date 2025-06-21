#include "Utilities/LimitedConcurrency.h"

nemesis::LimitedConcurrency::LimitedConcurrency(unsigned int max_threads) noexcept
    : MaxThreads(max_threads)
{
}

void nemesis::LimitedConcurrency::Lock()
{
    std::unique_lock<std::mutex> lock(Mutex);
    CondVar.wait(lock, [this] { return CurrentThreads < MaxThreads; });
    ++CurrentThreads;
}

void nemesis::LimitedConcurrency::Unlock()
{
    std::lock_guard<std::mutex> lock(Mutex);
    --CurrentThreads;
    CondVar.notify_one();
}
