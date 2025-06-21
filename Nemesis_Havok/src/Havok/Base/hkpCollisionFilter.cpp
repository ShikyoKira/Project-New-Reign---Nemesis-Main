#include "Havok/Base/hkpCollisionFilter.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCollisionFilter,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCollisionFilter::Class; });

nemesis::hkpCollisionFilter::hkpCollisionFilter() noexcept
    : nemesis::hkpCollisionFilter(Class)
{
}

nemesis::hkpCollisionFilter::hkpCollisionFilter(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
    , nemesis::hkpCollidableCollidableFilter(cls)
    , nemesis::hkpShapeCollectionFilter(cls)
    , nemesis::hkpRayShapeCollectionFilter(cls)
    , nemesis::hkpRayCollidableFilter(cls)
{
}

void nemesis::hkpCollisionFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);            // offset: 0/0 size: 8/16 align: 4/8
    nemesis::hkpCollidableCollidableFilter::SerializeTo(serializer); // offset: 8/16 size: 4/8 align: 4/8
    nemesis::hkpShapeCollectionFilter::SerializeTo(serializer);      // offset: 12/24 size: 4/8 align: 4/8
    nemesis::hkpRayShapeCollectionFilter::SerializeTo(serializer);   // offset: 16/32 size: 4/8 align: 4/8
    nemesis::hkpRayCollidableFilter::SerializeTo(serializer);        // offset: 20/40 size: 4/8 align: 4/8
    serializer.WriteValue("prepad", m_prepad);                       // offset: 24/48 size: 8/8 align: 4/4
    serializer.WriteValue("type", m_type);                           // offset: 32/56 size: 4/4 align: 4/4
    serializer.WriteValue("postpad", m_postpad);                     // offset: 36/60 size: 12/12 align: 4/4
    // class size: 48/72 align: 4/8
}

void nemesis::hkpCollisionFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    nemesis::hkpCollidableCollidableFilter::DeserializeFrom(
        deserializer);                                                   // offset: 8/16 size: 4/8 align: 4/8
    nemesis::hkpShapeCollectionFilter::DeserializeFrom(deserializer);    // offset: 12/24 size: 4/8 align: 4/8
    nemesis::hkpRayShapeCollectionFilter::DeserializeFrom(deserializer); // offset: 16/32 size: 4/8 align: 4/8
    nemesis::hkpRayCollidableFilter::DeserializeFrom(deserializer);      // offset: 20/40 size: 4/8 align: 4/8
    deserializer.ReadValue("prepad", m_prepad);                          // offset: 24/48 size: 8/8 align: 4/4
    deserializer.ReadValue("type", m_type);                              // offset: 32/56 size: 4/4 align: 4/4
    deserializer.ReadValue("postpad", m_postpad); // offset: 36/60 size: 12/12 align: 4/4
    // class size: 48/72 align: 4/8
}
