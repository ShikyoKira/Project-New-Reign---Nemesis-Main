#ifndef ATOMICLOCK_H_
#define ATOMICLOCK_H_

#include <boost/atomic.hpp>

class innerLock
{
	boost::atomic_flag* ulock;

public:

	innerLock(boost::atomic_flag& lock)
	{
		ulock = &lock;
		while (ulock->test_and_set(boost::memory_order_acquire));
	}

	~innerLock()
	{
		ulock->clear(boost::memory_order_release);
	}
};

#endif