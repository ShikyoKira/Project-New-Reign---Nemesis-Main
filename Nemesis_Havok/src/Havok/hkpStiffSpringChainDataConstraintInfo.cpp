#include "Havok/hkpStiffSpringChainDataConstraintInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpStiffSpringChainDataConstraintInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpStiffSpringChainDataConstraintInfo::Class; });

nemesis::hkpStiffSpringChainDataConstraintInfo::hkpStiffSpringChainDataConstraintInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpStiffSpringChainDataConstraintInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpStiffSpringChainDataConstraintInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("pivotInA", m_pivotInA);         // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("pivotInB", m_pivotInB);         // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("springLength", m_springLength); // offset: 32/32 size: 4/4 align: 4/4
    serializer.Skip(12);                                   // offset: 36/36 size: 12/12
    // class size: 48/48 align: 16/16
}

void nemesis::hkpStiffSpringChainDataConstraintInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("pivotInA", m_pivotInA);         // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("pivotInB", m_pivotInB);         // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("springLength", m_springLength); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.Skip(12);                                  // offset: 36/36 size: 12/12
    // class size: 48/48 align: 16/16
}
