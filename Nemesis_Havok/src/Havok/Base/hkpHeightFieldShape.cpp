#include "Havok/Base/hkpHeightFieldShape.h"

nemesis::hkpHeightFieldShape::hkpHeightFieldShape(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpShape(cls)
{
}

void nemesis::hkpHeightFieldShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShape::SerializeTo(serializer); // offset: 0/0 size: 16/32 align: 4/8
    // class size: 16/32 align: 4/8
}

void nemesis::hkpHeightFieldShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShape::DeserializeFrom(deserializer); // offset: 0/0 size: 16/32 align: 4/8
    // class size: 16/32 align: 4/8
}
