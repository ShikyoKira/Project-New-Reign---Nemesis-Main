#include "Global.h"

#include "ui/BehaviorInfo.h"

BehaviorInfo::BehaviorInfo(const QString name, const QString modauthor)
{
    modname = name;
    author  = modauthor;
}

BehaviorInfo::BehaviorInfo(const BehaviorInfo& other)
{
    modname = other.modname;
    author  = other.author;
    state   = other.state;
}
