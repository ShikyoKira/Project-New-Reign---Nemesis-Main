#include "Havok/hkbMirrorModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbMirrorModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbMirrorModifier::Class; });

nemesis::hkbMirrorModifier::hkbMirrorModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbMirrorModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbMirrorModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);     // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("isAdditive", m_isAdditive); // offset: 44/80 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());       // offset: 45/81 size: 3/7
    // class size: 48/88 align: 4/8
}

void nemesis::hkbMirrorModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("isAdditive", m_isAdditive);  // offset: 44/80 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());     // offset: 45/81 size: 3/7
    // class size: 48/88 align: 4/8
}
