#include "Havok/hkxNodeSelectionSet.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxNodeSelectionSet,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxNodeSelectionSet::Class; });

nemesis::hkxNodeSelectionSet::hkxNodeSelectionSet() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkxAttributeHolder(Class)
{
}

const nemesis::hkClass* nemesis::hkxNodeSelectionSet::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxNodeSelectionSet::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkxAttributeHolder::SerializeTo(serializer);     // offset: 0/0 size: 20/32 align: 4/8
    serializer.WriteObject("selectedNodes", m_selectedNodes); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteValue("name", m_name);                    // offset: 32/48 size: 4/8 align: 4/8
    // class size: 36/56 align: 4/8
}

void nemesis::hkxNodeSelectionSet::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkxAttributeHolder::DeserializeFrom(deserializer); // offset: 0/0 size: 20/32 align: 4/8
    deserializer.ReadObject("selectedNodes", m_selectedNodes);  // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadValue("name", m_name);                     // offset: 32/48 size: 4/8 align: 4/8
    // class size: 36/56 align: 4/8
}
