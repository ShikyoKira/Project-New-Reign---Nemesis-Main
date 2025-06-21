#include "Havok/Base/hkpRayShapeCollectionFilter.h"

nemesis::hkpRayShapeCollectionFilter::hkpRayShapeCollectionFilter(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

void nemesis::hkpRayShapeCollectionFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteUSize(0); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkpRayShapeCollectionFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadUSize(); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}
