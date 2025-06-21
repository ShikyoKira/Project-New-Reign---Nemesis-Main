#include "Havok/hkpShapeInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpShapeInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpShapeInfo::Class; });

nemesis::hkpShapeInfo::hkpShapeInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpShapeInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpShapeInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("shape", m_shape);             // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("isHierarchicalCompound",
                          m_isHierarchicalCompound);                   // offset: 12/24 size: 1/1 align: 1/1
    serializer.WriteValue("hkdShapesCollected", m_hkdShapesCollected); // offset: 13/25 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                       // offset: 14/26 size: 2/6

    serializer.WriteObject("childShapeNames", m_childShapeNames); // offset: 16/32 size: 12/16 align: 4/8
    serializer.WriteObject("childTransforms", m_childTransforms); // offset: 28/48 size: 12/16 align: 4/8
    serializer.Pad(16);                                           // offset: 40/64 size: 8/0

    serializer.WriteValue("transform", m_transform); // offset: 48/64 size: 64/64 align: 16/16
    // class size: 112/128 align: 16/16
}

void nemesis::hkpShapeInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("shape", m_shape);                  // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("isHierarchicalCompound",
                           m_isHierarchicalCompound);                   // offset: 12/24 size: 1/1 align: 1/1
    deserializer.ReadValue("hkdShapesCollected", m_hkdShapesCollected); // offset: 13/25 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 14/26 size: 2/6

    deserializer.ReadObject("childShapeNames", m_childShapeNames); // offset: 16/32 size: 12/16 align: 4/8
    deserializer.ReadObject("childTransforms", m_childTransforms); // offset: 28/48 size: 12/16 align: 4/8
    deserializer.Pad(16);                                          // offset: 40/64 size: 8/0

    deserializer.ReadValue("transform", m_transform); // offset: 48/64 size: 64/64 align: 16/16
    // class size: 112/128 align: 16/16
}
