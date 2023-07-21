#include "hkx/HkxEventList.h"

const nemesis::HkxEvent* nemesis::HkxEventList::GetEvent(const std::string& name) const noexcept
{
    return GetByName(name);
}

const nemesis::HkxEvent* nemesis::HkxEventList::GetEvent(size_t id) const noexcept
{
    return GetById(id);
}

bool nemesis::HkxEventList::HasEvent(const std::string& name) const noexcept
{
    return HasName(name);
}

bool nemesis::HkxEventList::HasEvent(size_t id) const noexcept
{
    return HasId(id);
}
