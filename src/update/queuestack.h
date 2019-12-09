#ifndef QUEUESTACK_H_
#define QUEUESTACK_H_

#include <boost\thread\mutex.hpp>

#include <memory>

template<typename T>
class QueuedVector : public std::vector<T>
{
	std::mutex lock;

public:
	void push_back(T _d)
	{
		std::lock_guard<std::mutex> wlock(lock);
		vector::push_back(_d);
	}

	T& at(size_t index)
	{
		std::lock_guard<std::mutex> rlock(lock);
		return vector::at(index);
	}

	T& operator[](size_t index)
	{
		std::lock_guard<std::mutex> rlock(lock);
		return vector::operator[](index);
	}

	void pop_back()
	{
		std::lock_guard<std::mutex> dlock(lock);
		vector::pop_back();
	}

	size_t size()
	{
		std::lock_guard<std::mutex> dlock(lock);
		return vector::size();
	}
};

#endif
