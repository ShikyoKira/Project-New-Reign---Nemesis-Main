#include "Havok/hkpConstrainedSystemFilter.h"

REGISTER_HAVOK_POINTER_SOURCE_2(hkpConstrainedSystemFilter,
                                hkReferencedObject,
                                [](nemesis::HavokVersion version)
                                { return &nemesis::hkpConstrainedSystemFilter::Class; });

nemesis::hkpConstrainedSystemFilter::hkpConstrainedSystemFilter() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCollisionFilter(Class)
    , nemesis::hkpConstraintListener(Class)
{
}

const nemesis::hkClass* nemesis::hkpConstrainedSystemFilter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstrainedSystemFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCollisionFilter::SerializeTo(serializer);    // offset: 0/0 size: 48/72 align: 4/8
    nemesis::hkpConstraintListener::SerializeTo(serializer); // offset: 48/72 size: 4/8 align: 4/8
    serializer.WriteObject("otherFilter", m_otherFilter);    // offset: 52/80 size: 4/8 align: 4/8
    // class size: 56/88 align: 4/8
}

void nemesis::hkpConstrainedSystemFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCollisionFilter::DeserializeFrom(deserializer);    // offset: 0/0 size: 48/72 align: 4/8
    nemesis::hkpConstraintListener::DeserializeFrom(deserializer); // offset: 48/72 size: 4/8 align: 4/8
    deserializer.ReadObject("otherFilter", m_otherFilter);         // offset: 52/80 size: 4/8 align: 4/8
    // class size: 56/88 align: 4/8
}
