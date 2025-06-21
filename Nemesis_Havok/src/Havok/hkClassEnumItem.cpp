#include "Havok/hkClass.h"
#include "Havok/hkClassEnumItem.h"

REGISTER_HAVOK_POINTER_SOURCE(hkClassEnumItem,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkClassEnumItem::Class; });

nemesis::hkClassEnumItem::hkClassEnumItem() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkClassEnumItem::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkClassEnumItem::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("value", m_value);     // offset: 0/0 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize()); // offset: 4/4 size: 0/4

    serializer.WriteValue("name", m_name); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkClassEnumItem::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("value", m_value);        // offset: 0/0 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 4/4 size: 0/4

    deserializer.ReadValue("name", m_name); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}
