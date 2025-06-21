#include "Havok/hkpShapeModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpShapeModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpShapeModifier::Class; });

nemesis::hkpShapeModifier::hkpShapeModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpShapeModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpShapeModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkpShapeModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
