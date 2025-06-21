#include "Havok/hkGizmoAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkGizmoAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkGizmoAttribute::Class; });

nemesis::hkGizmoAttribute::hkGizmoAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkGizmoAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkGizmoAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("visible", m_visible); // offset: 0/0 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 1/1 size: 3/7

    serializer.WriteValue("label", m_label);     // offset: 4/8 size: 4/8 align: 4/8
    serializer.WriteValue("type", m_type);       // offset: 8/16 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 9/17 size: 3/7
    // class size: 12/24 align: 4/8
}

void nemesis::hkGizmoAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("visible", m_visible);    // offset: 0/0 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 1/1 size: 3/7

    deserializer.ReadValue("label", m_label);        // offset: 4/8 size: 4/8 align: 4/8
    deserializer.ReadValue("type", m_type);          // offset: 8/16 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 9/17 size: 3/7
    // class size: 12/24 align: 4/8
}
