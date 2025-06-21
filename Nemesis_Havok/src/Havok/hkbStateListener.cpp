#include "Havok/hkbStateListener.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateListener,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateListener::Class; });

nemesis::hkbStateListener::hkbStateListener() noexcept
    : nemesis::hkbStateListener(Class)
{
}

nemesis::hkbStateListener::hkbStateListener(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkbStateListener::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateListener::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbStateListener::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
