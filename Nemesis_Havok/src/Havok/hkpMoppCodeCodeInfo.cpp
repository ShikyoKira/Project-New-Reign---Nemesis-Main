#include "Havok/hkpMoppCodeCodeInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMoppCodeCodeInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMoppCodeCodeInfo::Class; });

nemesis::hkpMoppCodeCodeInfo::hkpMoppCodeCodeInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpMoppCodeCodeInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMoppCodeCodeInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("offset", m_offset); // offset: 0/0 size: 16/16 align: 16/16
    // class size: 16/16 align: 16/16
}

void nemesis::hkpMoppCodeCodeInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("offset", m_offset); // offset: 0/0 size: 16/16 align: 16/16
    // class size: 16/16 align: 16/16
}
