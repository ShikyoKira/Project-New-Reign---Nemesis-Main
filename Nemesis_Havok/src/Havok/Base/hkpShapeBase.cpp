#include "Havok/Base/hkpShapeBase.h"

nemesis::hkpShapeBase::hkpShapeBase(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkcdShape(cls)
{
}

void nemesis::hkpShapeBase::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkcdShape::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkpShapeBase::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkcdShape::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    // class size: 12/24 align: 4/8
}
