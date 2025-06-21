#include "Havok/hkpConvexTranslateShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConvexTranslateShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConvexTranslateShape::Class; });

nemesis::hkpConvexTranslateShape::hkpConvexTranslateShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexTransformShapeBase(Class)
{
}

const nemesis::hkClass* nemesis::hkpConvexTranslateShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConvexTranslateShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexTransformShapeBase::SerializeTo(serializer); // offset: 0/0 size: 36/64 align: 4/8
    serializer.Pad(16);                                            // offset: 36/64 size: 12/0

    serializer.WriteValue("translation", m_translation); // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

void nemesis::hkpConvexTranslateShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexTransformShapeBase::DeserializeFrom(deserializer); // offset: 0/0 size: 36/64 align: 4/8
    deserializer.Pad(16);                                                // offset: 36/64 size: 12/0

    deserializer.ReadValue("translation", m_translation); // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}
