#include "hkx/HkxAttributeList.h"

const nemesis::HkxAttribute* nemesis::HkxAttributeList::GetVariable(const std::string& name) const noexcept
{
    return GetByName(name);
}

const nemesis::HkxAttribute* nemesis::HkxAttributeList::GetVariable(size_t id) const noexcept
{
    return GetById(id);
}

bool nemesis::HkxAttributeList::HasVariable(const std::string& name) const noexcept
{
    return HasName(name);
}

bool nemesis::HkxAttributeList::HasVariable(size_t id) const noexcept
{
    return HasId(id);
}
