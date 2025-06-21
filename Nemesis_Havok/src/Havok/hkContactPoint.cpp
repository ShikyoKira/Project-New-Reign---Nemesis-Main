#include "Havok/hkContactPoint.h"

REGISTER_HAVOK_POINTER_SOURCE(hkContactPoint,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkContactPoint::Class; });

nemesis::hkContactPoint::hkContactPoint() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkContactPoint::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkContactPoint::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("position", m_position);                 // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("separatingNormal", m_separatingNormal); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

void nemesis::hkContactPoint::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("position", m_position);                 // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("separatingNormal", m_separatingNormal); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}
