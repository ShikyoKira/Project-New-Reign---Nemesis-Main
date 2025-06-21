#include "Havok/hkbVariableBounds.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbVariableBounds,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbVariableBounds::Class; });

nemesis::hkbVariableBounds::hkbVariableBounds() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbVariableBounds::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbVariableBounds::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("min", m_min); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteObject("max", m_max); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkbVariableBounds::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("min", m_min); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadObject("max", m_max); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}
