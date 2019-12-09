#ifndef ATOMICLOCK_H_
#define ATOMICLOCK_H_

#include <boost/atomic.hpp>

class Lockless
{
	boost::atomic_flag* ulock;

public:
	Lockless(boost::atomic_flag& lock)
	{
		ulock = &lock;
		while (ulock->test_and_set(boost::memory_order_acquire));
	}

	~Lockless()
	{
		if (ulock == nullptr) return;

		ulock->clear(boost::memory_order_release);
	}

	void Unlock()
	{
		ulock->clear(boost::memory_order_release);
		ulock = nullptr;
	}
};

#endif