#include "Havok/hkpListShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpListShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpListShape::Class; });

nemesis::hkpListShape::hkpListShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShapeCollection(Class)
{
}

const nemesis::hkClass* nemesis::hkpListShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpListShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShapeCollection::SerializeTo(serializer); // offset: 0/0 size: 24/48 align: 4/8
    serializer.WriteObject("childInfo", m_childInfo);     // offset: 24/48 size: 12/16 align: 4/8
    serializer.WriteValue("flags", m_flags);              // offset: 36/64 size: 2/2 align: 2/2
    serializer.WriteValue("numDisabledChildren", m_numDisabledChildren); // offset: 38/66 size: 2/2 align: 2/2
    serializer.Pad(16);                                                  // offset: 40/68 size: 8/12

    serializer.WriteValue("aabbHalfExtents", m_aabbHalfExtents); // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("aabbCenter", m_aabbCenter);           // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("enabledChildren", m_enabledChildren); // offset: 80/112 size: 32/32 align: 4/4
    // class size: 112/144 align: 16/16
}

void nemesis::hkpListShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShapeCollection::DeserializeFrom(deserializer); // offset: 0/0 size: 24/48 align: 4/8
    deserializer.ReadObject("childInfo", m_childInfo);          // offset: 24/48 size: 12/16 align: 4/8
    deserializer.ReadValue("flags", m_flags);                   // offset: 36/64 size: 2/2 align: 2/2
    deserializer.ReadValue("numDisabledChildren",
                           m_numDisabledChildren); // offset: 38/66 size: 2/2 align: 2/2
    deserializer.Pad(16);                          // offset: 40/68 size: 8/12

    deserializer.ReadValue("aabbHalfExtents", m_aabbHalfExtents); // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("aabbCenter", m_aabbCenter);           // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("enabledChildren", m_enabledChildren); // offset: 80/112 size: 32/32 align: 4/4
    // class size: 112/144 align: 16/16
}
