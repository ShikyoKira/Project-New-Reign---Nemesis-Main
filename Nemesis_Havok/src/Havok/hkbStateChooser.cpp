#include "Havok/hkbStateChooser.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateChooser,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateChooser::Class; });

nemesis::hkbStateChooser::hkbStateChooser() noexcept
    : nemesis::hkbStateChooser(Class)
{
}

nemesis::hkbStateChooser::hkbStateChooser(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkbStateChooser::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateChooser::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbStateChooser::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
