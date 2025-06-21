#include "Havok/hkpPhantomCallbackShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPhantomCallbackShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPhantomCallbackShape::Class; });

nemesis::hkpPhantomCallbackShape::hkpPhantomCallbackShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpPhantomCallbackShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPhantomCallbackShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShape::SerializeTo(serializer); // offset: 0/0 size: 16/32 align: 4/8
    // class size: 16/32 align: 4/8
}

void nemesis::hkpPhantomCallbackShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShape::DeserializeFrom(deserializer); // offset: 0/0 size: 16/32 align: 4/8
    // class size: 16/32 align: 4/8
}
