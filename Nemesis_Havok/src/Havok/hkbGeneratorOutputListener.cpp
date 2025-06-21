#include "Havok/hkbGeneratorOutputListener.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGeneratorOutputListener,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGeneratorOutputListener::Class; });

nemesis::hkbGeneratorOutputListener::hkbGeneratorOutputListener() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbGeneratorOutputListener::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGeneratorOutputListener::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbGeneratorOutputListener::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
