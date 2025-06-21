#include "Havok/hkReflectedFileAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkReflectedFileAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkReflectedFileAttribute::Class; });

nemesis::hkReflectedFileAttribute::hkReflectedFileAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkReflectedFileAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkReflectedFileAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("value", m_value); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkReflectedFileAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("value", m_value); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}
