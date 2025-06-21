#include "Havok/Base/HavokObject.h"

const nemesis::hkClass* nemesis::HavokObject::GetClass(nemesis::HavokVersion version) const
{
    return nullptr;
}

UMap<std::string, std::function<nemesis::HavokObject*()>>& nemesis::HavokObject::GetCreationMap()
{
    static UMap<std::string, std::function<nemesis::HavokObject*()>> creation_map;
    return creation_map;
}

UMap<std::string, std::function<const nemesis::hkClass*(nemesis::HavokVersion)>>&
nemesis::HavokObject::GetClassMap()
{
    static UMap<std::string, std::function<const nemesis::hkClass*(nemesis::HavokVersion)>> class_map;
    return class_map;
}
