#include "Havok/hkbVariableInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbVariableInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbVariableInfo::Class; });

nemesis::hkbVariableInfo::hkbVariableInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbVariableInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbVariableInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("role", m_role); // offset: 0/0 size: 4/4 align: 2/2
    serializer.WriteValue("type", m_type);  // offset: 4/4 size: 1/1 align: 1/1
    serializer.Skip(1);                     // offset: 5/5 size: 1/1
    // class size: 6/6 align: 2/2
}

void nemesis::hkbVariableInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("role", m_role); // offset: 0/0 size: 4/4 align: 2/2
    deserializer.ReadValue("type", m_type);  // offset: 4/4 size: 1/1 align: 1/1
    deserializer.Skip(1);                    // offset: 5/5 size: 1/1
    // class size: 6/6 align: 2/2
}
