#include "hkx/attrlist.h"

const nemesis::AttrId* nemesis::AttrList::GetVariable(const std::string& name) const noexcept
{
    return GetByName(name);
}

const nemesis::AttrId* nemesis::AttrList::GetVariable(size_t id) const noexcept
{
    return GetById(id);
}

bool nemesis::AttrList::HasVariable(const std::string& name) const noexcept
{
    return HasName(name);
}

bool nemesis::AttrList::HasVariable(size_t id) const noexcept
{
    return HasId(id);
}
