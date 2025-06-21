#include "Havok/hkbAttributeModifierAssignment.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbAttributeModifierAssignment,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbAttributeModifierAssignment::Class; });

nemesis::hkbAttributeModifierAssignment::hkbAttributeModifierAssignment() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbAttributeModifierAssignment::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbAttributeModifierAssignment::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("attributeIndex", m_attributeIndex); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("attributeValue", m_attributeValue); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkbAttributeModifierAssignment::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("attributeIndex", m_attributeIndex); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("attributeValue", m_attributeValue); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}
