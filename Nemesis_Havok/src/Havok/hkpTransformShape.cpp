#include "Havok/hkpTransformShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpTransformShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpTransformShape::Class; });

nemesis::hkpTransformShape::hkpTransformShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpTransformShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpTransformShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShape::SerializeTo(serializer);         // offset: 0/0 size: 16/32 align: 4/8
    serializer.WriteObject("childShape", m_childShape); // offset: 16/32 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("childShapeSize",
                                          m_childShapeSize); // offset: 28/48 size: 4/4 align: 4/4
    serializer.Pad(16);                                      // offset: 32/52 size: 0/12

    serializer.WriteValue("rotation", m_rotation);   // offset: 32/64 size: 16/16 align: 16/16
    serializer.WriteValue("transform", m_transform); // offset: 48/80 size: 64/64 align: 16/16
    // class size: 112/144 align: 16/16
}

void nemesis::hkpTransformShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShape::DeserializeFrom(deserializer);           // offset: 0/0 size: 16/32 align: 4/8
    deserializer.ReadObject("childShape", m_childShape);        // offset: 16/32 size: 12/16 align: 4/8
    deserializer.ReadValue("childShapeSize", m_childShapeSize); // offset: 28/48 size: 4/4 align: 4/4
    deserializer.Pad(16);                                       // offset: 32/52 size: 0/12

    deserializer.ReadValue("rotation", m_rotation);   // offset: 32/64 size: 16/16 align: 16/16
    deserializer.ReadValue("transform", m_transform); // offset: 48/80 size: 64/64 align: 16/16
    // class size: 112/144 align: 16/16
}
