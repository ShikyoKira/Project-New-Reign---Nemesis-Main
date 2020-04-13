#include "Global.h"

#include "generate/animation/animationthread.h"

using namespace std;

void NewAnimLock::lockCore()
{
    while (coreLock.test_and_set(std::memory_order_acquire))
        ;
}

void NewAnimLock::releaseCore()
{
    coreLock.clear(std::memory_order_release);
}

void NewAnimLock::lockID()
{
    while (subIDLock.test_and_set(std::memory_order_acquire))
        ;
}

void NewAnimLock::releaseID()
{
    subIDLock.clear(std::memory_order_release);
}

void NewAnimLock::lockExport()
{
    while (exportLock.test_and_set(std::memory_order_acquire))
        ;
}

void NewAnimLock::releaseExport()
{
    exportLock.clear(std::memory_order_release);
}
