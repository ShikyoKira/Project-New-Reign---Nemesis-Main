#pragma once

#include <atomic>

namespace nemesis
{
    class Lockless
    {
        std::atomic_flag* lock;

    public:
        Lockless(std::atomic_flag& _lock)
        {
            lock = &_lock;

            while (lock->test_and_set(std::memory_order_acquire))
                ;
        }

        ~Lockless()
        {
            if (lock == nullptr) return;

            lock->clear(std::memory_order_release);
            lock = nullptr;
        }

        void Unlock()
        {
            if (lock == nullptr) return;

            lock->clear(std::memory_order_release);
            lock = nullptr;
        }
    };
}
