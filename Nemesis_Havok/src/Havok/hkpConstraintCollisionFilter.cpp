#include "Havok/hkpConstraintCollisionFilter.h"

REGISTER_HAVOK_POINTER_SOURCE_2(hkpConstraintCollisionFilter,
                                hkReferencedObject,
                                [](nemesis::HavokVersion version)
                                { return &nemesis::hkpConstraintCollisionFilter::Class; });

nemesis::hkpConstraintCollisionFilter::hkpConstraintCollisionFilter() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpPairCollisionFilter(Class)
    , nemesis::hkpConstraintListener(Class)
{
}

const nemesis::hkClass* nemesis::hkpConstraintCollisionFilter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstraintCollisionFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpPairCollisionFilter::SerializeTo(serializer); // offset: 0/0 size: 64/96 align: 4/8
    nemesis::hkpConstraintListener::SerializeTo(serializer);  // offset: 64/96 size: 4/8 align: 4/8
    // class size: 68/104 align: 4/8
}

void nemesis::hkpConstraintCollisionFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpPairCollisionFilter::DeserializeFrom(deserializer); // offset: 0/0 size: 64/96 align: 4/8
    nemesis::hkpConstraintListener::DeserializeFrom(deserializer);  // offset: 64/96 size: 4/8 align: 4/8
    // class size: 68/104 align: 4/8
}
