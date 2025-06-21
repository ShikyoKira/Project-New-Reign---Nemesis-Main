#include "Havok/hkpTypedBroadPhaseHandle.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpTypedBroadPhaseHandle,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpTypedBroadPhaseHandle::Class; });

nemesis::hkpTypedBroadPhaseHandle::hkpTypedBroadPhaseHandle() noexcept
    : nemesis::hkpBroadPhaseHandle(Class)
{
}

const nemesis::hkClass* nemesis::hkpTypedBroadPhaseHandle::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpTypedBroadPhaseHandle::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpBroadPhaseHandle::SerializeTo(serializer);               // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("type", m_type);                               // offset: 4/4 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("ownerOffset", m_ownerOffset); // offset: 5/5 size: 1/1 align: 1/1
    serializer.WriteValue("objectQualityType", m_objectQualityType);     // offset: 6/6 size: 1/1 align: 1/1
    serializer.Skip(1);                                                  // offset: 7/7 size: 1/1

    serializer.WriteValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 8/8 size: 4/4 align: 4/4
    // class size: 12/12 align: 4/4
}

void nemesis::hkpTypedBroadPhaseHandle::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpBroadPhaseHandle::DeserializeFrom(deserializer);      // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("type", m_type);                           // offset: 4/4 size: 1/1 align: 1/1
    deserializer.ReadValue("ownerOffset", m_ownerOffset);             // offset: 5/5 size: 1/1 align: 1/1
    deserializer.ReadValue("objectQualityType", m_objectQualityType); // offset: 6/6 size: 1/1 align: 1/1
    deserializer.Skip(1);                                             // offset: 7/7 size: 1/1

    deserializer.ReadValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 8/8 size: 4/4 align: 4/4
    // class size: 12/12 align: 4/4
}
