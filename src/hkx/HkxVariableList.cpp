#include "hkx/HkxVariableList.h"

const nemesis::HkxVariable* nemesis::HkxVariableList::GetVariable(const std::string& name) const noexcept
{
    return GetByName(name);
}

const nemesis::HkxVariable* nemesis::HkxVariableList::GetVariable(size_t id) const noexcept
{
    return GetById(id);
}

bool nemesis::HkxVariableList::HasVariable(const std::string& name) const noexcept
{
    return HasName(name);
}

bool nemesis::HkxVariableList::HasVariable(size_t id) const noexcept
{
    return HasId(id);
}
