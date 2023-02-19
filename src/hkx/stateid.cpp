#include "hkx/stateid.h"

#include "utilities/atomiclock.h"

nemesis::StateId::StateId(size_t startid)
{
    this->startid = startid;
}

size_t nemesis::StateId::GenerateNewId()
{
    Lockless lockless(lock);
    return startid++;
}
