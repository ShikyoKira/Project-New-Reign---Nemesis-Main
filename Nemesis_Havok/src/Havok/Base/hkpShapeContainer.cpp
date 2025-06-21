#include "Havok/Base/hkpShapeContainer.h"

nemesis::hkpShapeContainer::hkpShapeContainer(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

void nemesis::hkpShapeContainer::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteUSize(0); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkpShapeContainer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadUSize(); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}
