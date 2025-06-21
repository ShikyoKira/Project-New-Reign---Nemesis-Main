#include "AlternateAnimation/AlterAnimPrefix.h"

nemesis::AlterAnimPrefix::AlterAnimPrefix(size_t id, const std::string& name)
    : Id(id)
    , Name(name)
{
}

size_t nemesis::AlterAnimPrefix::GetId() const noexcept
{
    return Id;
}

const std::string& nemesis::AlterAnimPrefix::GetName() const noexcept
{
    return Name;
}
