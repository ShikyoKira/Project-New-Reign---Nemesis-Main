#include "Havok/hkpMoppCode.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMoppCode,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMoppCode::Class; });

nemesis::hkpMoppCode::hkpMoppCode() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpMoppCode::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMoppCode::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteObject("info", m_info);          // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteObject("data", m_data);          // offset: 32/32 size: 12/16 align: 4/8
    serializer.WriteValue("buildType", m_buildType); // offset: 44/48 size: 1/1 align: 1/1
    serializer.Pad(16);                              // offset: 45/49 size: 3/15
    // class size: 48/64 align: 16/16
}

void nemesis::hkpMoppCode::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadObject("info", m_info);          // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadObject("data", m_data);          // offset: 32/32 size: 12/16 align: 4/8
    deserializer.ReadValue("buildType", m_buildType); // offset: 44/48 size: 1/1 align: 1/1
    deserializer.Pad(16);                             // offset: 45/49 size: 3/15
    // class size: 48/64 align: 16/16
}
