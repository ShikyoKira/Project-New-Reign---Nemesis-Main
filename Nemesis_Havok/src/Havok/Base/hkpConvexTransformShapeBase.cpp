#include "Havok/Base/hkpConvexTransformShapeBase.h"

nemesis::hkpConvexTransformShapeBase::hkpConvexTransformShapeBase(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpConvexShape(cls)
{
}

const nemesis::hkClass* nemesis::hkpConvexTransformShapeBase::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConvexTransformShapeBase::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexShape::SerializeTo(serializer);   // offset: 0/0 size: 20/40 align: 4/8
    serializer.WriteObject("childShape", m_childShape); // offset: 20/40 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("childShapeSize",
                                          m_childShapeSize); // offset: 32/56 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());             // offset: 36/60 size: 0/4
    // class size: 36/64 align: 4/8
}

void nemesis::hkpConvexTransformShapeBase::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexShape::DeserializeFrom(deserializer);     // offset: 0/0 size: 20/40 align: 4/8
    deserializer.ReadObject("childShape", m_childShape);        // offset: 20/40 size: 12/16 align: 4/8
    deserializer.ReadValue("childShapeSize", m_childShapeSize); // offset: 32/56 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 36/60 size: 0/4
    // class size: 36/64 align: 4/8
}
