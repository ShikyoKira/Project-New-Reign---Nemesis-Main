#include "Havok/Base/hkBaseObject.h"

nemesis::hkBaseObject::hkBaseObject(unsigned int signature) noexcept
    : nemesis::HavokObject(signature)
{
}

void nemesis::hkBaseObject::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteUSize(0); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkBaseObject::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadUSize(); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}
