#ifndef ATOMICLOCK_H_
#define ATOMICLOCK_H_

#include <atomic>

#include <boost/atomic.hpp>

class Lockless
{
	boost::atomic_flag* lock;

public:
	Lockless(boost::atomic_flag& _lock)
	{
		lock = &_lock;

		while (lock->test_and_set(boost::memory_order_acquire));
	}

	~Lockless()
	{
		if (lock == nullptr) return;

		lock->clear(boost::memory_order_release);
		lock = nullptr;
	}

	void Unlock()
	{
		if (lock == nullptr) return;

		lock->clear(boost::memory_order_release);
		lock = nullptr;
	}
};

class Lockless_s
{
	std::atomic_flag* lock;

public:
	Lockless_s(std::atomic_flag& _lock)
	{
		lock = &_lock;

		while (lock->test_and_set(std::memory_order_acquire));
	}

	~Lockless_s()
	{
		if (lock == nullptr) return;

		lock->clear(std::memory_order_release);
		lock = nullptr;
	}

	void Unlock()
	{
		lock->clear(std::memory_order_release);
		lock = nullptr;
	}
};
#endif