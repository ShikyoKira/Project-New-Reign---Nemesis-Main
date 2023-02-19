#include "hkx/eventlist.h"

const nemesis::EventId* nemesis::EventList::GetEvent(const std::string& name) const noexcept
{
    return GetByName(name);
}

const nemesis::EventId* nemesis::EventList::GetEvent(size_t id) const noexcept
{
    return GetById(id);
}

bool nemesis::EventList::HasEvent(const std::string& name) const noexcept
{
    return HasName(name);
}

bool nemesis::EventList::HasEvent(size_t id) const noexcept
{
    return HasId(id);
}
