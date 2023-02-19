#include "base/havokdata.h"

nemesis::HavokData::HavokData(const std::string& name, size_t id)
{
    this->name = name;
    this->id   = id;
}

std::string nemesis::HavokData::GetName() const noexcept
{
    return name;
}

size_t nemesis::HavokData::GetId() const noexcept
{
    return id;
}
