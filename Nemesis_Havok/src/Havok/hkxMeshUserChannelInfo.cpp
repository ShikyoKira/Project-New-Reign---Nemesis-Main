#include "Havok/hkxMeshUserChannelInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMeshUserChannelInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMeshUserChannelInfo::Class; });

nemesis::hkxMeshUserChannelInfo::hkxMeshUserChannelInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkxAttributeHolder(Class)
{
}

const nemesis::hkClass* nemesis::hkxMeshUserChannelInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxMeshUserChannelInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkxAttributeHolder::SerializeTo(serializer); // offset: 0/0 size: 20/32 align: 4/8
    serializer.WriteValue("name", m_name);                // offset: 20/32 size: 4/8 align: 4/8
    serializer.WriteValue("className", m_className);      // offset: 24/40 size: 4/8 align: 4/8
    // class size: 28/48 align: 4/8
}

void nemesis::hkxMeshUserChannelInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkxAttributeHolder::DeserializeFrom(deserializer); // offset: 0/0 size: 20/32 align: 4/8
    deserializer.ReadValue("name", m_name);                     // offset: 20/32 size: 4/8 align: 4/8
    deserializer.ReadValue("className", m_className);           // offset: 24/40 size: 4/8 align: 4/8
    // class size: 28/48 align: 4/8
}
