#include "Havok/hkpNullCollisionFilter.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpNullCollisionFilter,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpNullCollisionFilter::Class; });

nemesis::hkpNullCollisionFilter::hkpNullCollisionFilter() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCollisionFilter(Class)
{
}

const nemesis::hkClass* nemesis::hkpNullCollisionFilter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpNullCollisionFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCollisionFilter::SerializeTo(serializer); // offset: 0/0 size: 48/72 align: 4/8
    // class size: 48/72 align: 4/8
}

void nemesis::hkpNullCollisionFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCollisionFilter::DeserializeFrom(deserializer); // offset: 0/0 size: 48/72 align: 4/8
    // class size: 48/72 align: 4/8
}
