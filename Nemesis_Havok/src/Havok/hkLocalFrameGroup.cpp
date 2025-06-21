#include "Havok/hkLocalFrameGroup.h"

REGISTER_HAVOK_POINTER_SOURCE(hkLocalFrameGroup,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkLocalFrameGroup::Class; });

nemesis::hkLocalFrameGroup::hkLocalFrameGroup() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkLocalFrameGroup::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkLocalFrameGroup::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("name", m_name);                // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkLocalFrameGroup::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("name", m_name);                     // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}
