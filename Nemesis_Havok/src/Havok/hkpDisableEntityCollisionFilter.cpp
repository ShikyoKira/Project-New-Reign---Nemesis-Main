#include "Havok/hkpDisableEntityCollisionFilter.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpDisableEntityCollisionFilter,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpDisableEntityCollisionFilter::Class; });

nemesis::hkpDisableEntityCollisionFilter::hkpDisableEntityCollisionFilter() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCollisionFilter(Class)
    , nemesis::hkpEntityListener(Class)
{
}

const nemesis::hkClass* nemesis::hkpDisableEntityCollisionFilter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpDisableEntityCollisionFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCollisionFilter::SerializeTo(serializer);           // offset: 0/0 size: 48/72 align: 4/8
    nemesis::hkpEntityListener::SerializeTo(serializer);            // offset: 48/72 size: 4/8 align: 4/8
    serializer.WriteObject("disabledEntities", m_disabledEntities); // offset: 52/80 size: 12/16 align: 4/8
    // class size: 64/96 align: 4/8
}

void nemesis::hkpDisableEntityCollisionFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCollisionFilter::DeserializeFrom(deserializer);      // offset: 0/0 size: 48/72 align: 4/8
    nemesis::hkpEntityListener::DeserializeFrom(deserializer);       // offset: 48/72 size: 4/8 align: 4/8
    deserializer.ReadObject("disabledEntities", m_disabledEntities); // offset: 52/80 size: 12/16 align: 4/8
    // class size: 64/96 align: 4/8
}
