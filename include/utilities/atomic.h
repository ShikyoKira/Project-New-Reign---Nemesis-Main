/*
USAGE

Atomic<int> atomic_int;

atomic_int.Write().AsObject() = 5;

*/

#pragma once

#include <atomic>

namespace nemesis
{
    template<typename T>
	struct Atomic
    {
        struct Writer
        {
            std::atomic_flag& lock;
            T& value;

            Writer(Atomic<T>& ref)
                : value(ref.value)
                , lock(ref.lock)
            {
                while(lock.test_and_set(std::memory_order_acquire));
            }

            ~Writer()
            {
                lock.clear(std::memory_order_release);
            }

            T& AsObject()
            {
                return value;
            }
        };

    private:
        mutable std::atomic_flag lock;
        T value;

    public:
        Atomic() = default;
        Atomic(const T& value)
        {
            this->value = value;
        }
        
        const T& operator=(const T& value)
        {
            this->value = value;
            return this->value;
        }

        const T& Read() const
        {
            return value;
        }

        Writer Write()
        {
            return Writer(*this);
        }
    };
}
