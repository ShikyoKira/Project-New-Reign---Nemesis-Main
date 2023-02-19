#include "hkx/varlist.h"

const nemesis::VarId* nemesis::VarList::GetVariable(const std::string& name) const noexcept
{
    return GetByName(name);
}

const nemesis::VarId* nemesis::VarList::GetVariable(size_t id) const noexcept
{
    return GetById(id);
}

bool nemesis::VarList::HasVariable(const std::string& name) const noexcept
{
    return HasName(name);
}

bool nemesis::VarList::HasVariable(size_t id) const noexcept
{
    return HasId(id);
}
