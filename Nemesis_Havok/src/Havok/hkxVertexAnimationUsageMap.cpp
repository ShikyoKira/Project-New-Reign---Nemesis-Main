#include "Havok/hkxVertexAnimationUsageMap.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexAnimationUsageMap,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexAnimationUsageMap::Class; });

nemesis::hkxVertexAnimationUsageMap::hkxVertexAnimationUsageMap() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxVertexAnimationUsageMap::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexAnimationUsageMap::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("use", m_use);                     // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("useIndexOrig", m_useIndexOrig);   // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("useIndexLocal", m_useIndexLocal); // offset: 3/3 size: 1/1 align: 1/1
    // class size: 4/4 align: 2/2
}

void nemesis::hkxVertexAnimationUsageMap::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("use", m_use);                     // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("useIndexOrig", m_useIndexOrig);   // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("useIndexLocal", m_useIndexLocal); // offset: 3/3 size: 1/1 align: 1/1
    // class size: 4/4 align: 2/2
}
