#include "Havok/hkpGroupFilter.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpGroupFilter,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpGroupFilter::Class; });

nemesis::hkpGroupFilter::hkpGroupFilter() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCollisionFilter(Class)
{
}

const nemesis::hkClass* nemesis::hkpGroupFilter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpGroupFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCollisionFilter::SerializeTo(serializer);                // offset: 0/0 size: 48/72 align: 4/8
    serializer.WriteValue("nextFreeSystemGroup", m_nextFreeSystemGroup); // offset: 48/72 size: 4/4 align: 4/4
    serializer.WriteValue("collisionLookupTable",
                          m_collisionLookupTable); // offset: 52/76 size: 128/128 align: 4/4
    serializer.Pad(16);                            // offset: 180/204 size: 12/4

    serializer.WriteValue("pad256", m_pad256); // offset: 192/208 size: 64/64 align: 16/16
    // class size: 256/272 align: 16/16
}

void nemesis::hkpGroupFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCollisionFilter::DeserializeFrom(deserializer); // offset: 0/0 size: 48/72 align: 4/8
    deserializer.ReadValue("nextFreeSystemGroup",
                           m_nextFreeSystemGroup); // offset: 48/72 size: 4/4 align: 4/4
    deserializer.ReadValue("collisionLookupTable",
                           m_collisionLookupTable); // offset: 52/76 size: 128/128 align: 4/4
    deserializer.Pad(16);                           // offset: 180/204 size: 12/4

    deserializer.ReadValue("pad256", m_pad256); // offset: 192/208 size: 64/64 align: 16/16
    // class size: 256/272 align: 16/16
}
