#include "animationthread.h"

using namespace std;

void newAnimLock::lockCore()
{
	while (coreLock.test_and_set(std::memory_order_acquire));
}

void newAnimLock::releaseCore()
{
	coreLock.clear(std::memory_order_release);
}

void newAnimLock::lockID()
{
	while (subIDLock.test_and_set(std::memory_order_acquire));
}

void newAnimLock::releaseID()
{
	subIDLock.clear(std::memory_order_release);
}

void newAnimLock::lockExport()
{
	while (exportLock.test_and_set(std::memory_order_acquire));
}

void newAnimLock::releaseExport()
{
	exportLock.clear(std::memory_order_release);
}
