#include "Havok/hkpPairCollisionFilter.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPairCollisionFilter,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPairCollisionFilter::Class; });

nemesis::hkpPairCollisionFilter::hkpPairCollisionFilter() noexcept
    : nemesis::hkpPairCollisionFilter(Class)
{
}

nemesis::hkpPairCollisionFilter::hkpPairCollisionFilter(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpCollisionFilter(cls)
{
}

const nemesis::hkClass* nemesis::hkpPairCollisionFilter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPairCollisionFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCollisionFilter::SerializeTo(serializer); // offset: 0/0 size: 48/72 align: 4/8
    serializer.WriteSerializeIgnoredObject("disabledPairs",
                                           m_disabledPairs); // offset: 48/72 size: 12/16 align: 4/8
    serializer.WriteObject("childFilter", m_childFilter);    // offset: 60/88 size: 4/8 align: 4/8
    // class size: 64/96 align: 4/8
}

void nemesis::hkpPairCollisionFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCollisionFilter::DeserializeFrom(deserializer); // offset: 0/0 size: 48/72 align: 4/8
    deserializer.ReadObject("disabledPairs", m_disabledPairs);  // offset: 48/72 size: 12/16 align: 4/8
    deserializer.ReadObject("childFilter", m_childFilter);      // offset: 60/88 size: 4/8 align: 4/8
    // class size: 64/96 align: 4/8
}
