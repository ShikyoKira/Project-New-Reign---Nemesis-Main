#include "hkx/base/HavokData.h"

nemesis::HavokData::HavokData(const std::string& name, size_t id)
    : Name(name)
    , Id(id)
{
}

std::string nemesis::HavokData::GetName() const noexcept
{
    return Name;
}

size_t nemesis::HavokData::GetId() const noexcept
{
    return Id;
}
