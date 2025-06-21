#include "Havok/hkSemanticsAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkSemanticsAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkSemanticsAttribute::Class; });

nemesis::hkSemanticsAttribute::hkSemanticsAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkSemanticsAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkSemanticsAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("type", m_type); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkSemanticsAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("type", m_type); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}
