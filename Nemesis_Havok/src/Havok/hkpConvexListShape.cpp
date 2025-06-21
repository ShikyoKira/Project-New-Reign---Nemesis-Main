#include "Havok/hkpConvexListShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConvexListShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConvexListShape::Class; });

nemesis::hkpConvexListShape::hkpConvexListShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexShape(Class)
    , nemesis::hkpShapeContainer(Class)
{
}

const nemesis::hkClass* nemesis::hkpConvexListShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConvexListShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexShape::SerializeTo(serializer);    // offset: 0/0 size: 20/40 align: 4/8
    nemesis::hkpShapeContainer::SerializeTo(serializer); // offset: 20/40 size: 4/8 align: 4/8
    serializer.WriteValue(
        "minDistanceToUseConvexHullForGetClosestPoints",
        m_minDistanceToUseConvexHullForGetClosestPoints); // offset: 24/48 size: 4/4 align: 4/4
    serializer.Pad(16);                                   // offset: 28/52 size: 4/12

    serializer.WriteValue("aabbHalfExtents", m_aabbHalfExtents); // offset: 32/64 size: 16/16 align: 16/16
    serializer.WriteValue("aabbCenter", m_aabbCenter);           // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("useCachedAabb", m_useCachedAabb);     // offset: 64/96 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                 // offset: 65/97 size: 3/7

    serializer.WriteObject("childShapes", m_childShapes); // offset: 68/104 size: 12/16 align: 4/8
    serializer.Pad(16);                                   // offset: 80/120 size: 0/8
    // class size: 80/128 align: 16/16
}

void nemesis::hkpConvexListShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexShape::DeserializeFrom(deserializer);    // offset: 0/0 size: 20/40 align: 4/8
    nemesis::hkpShapeContainer::DeserializeFrom(deserializer); // offset: 20/40 size: 4/8 align: 4/8
    deserializer.ReadValue(
        "minDistanceToUseConvexHullForGetClosestPoints",
        m_minDistanceToUseConvexHullForGetClosestPoints); // offset: 24/48 size: 4/4 align: 4/4
    deserializer.Pad(16);                                 // offset: 28/52 size: 4/12

    deserializer.ReadValue("aabbHalfExtents", m_aabbHalfExtents); // offset: 32/64 size: 16/16 align: 16/16
    deserializer.ReadValue("aabbCenter", m_aabbCenter);           // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("useCachedAabb", m_useCachedAabb);     // offset: 64/96 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 65/97 size: 3/7

    deserializer.ReadObject("childShapes", m_childShapes); // offset: 68/104 size: 12/16 align: 4/8
    deserializer.Pad(16);                                  // offset: 80/120 size: 0/8
    // class size: 80/128 align: 16/16
}
