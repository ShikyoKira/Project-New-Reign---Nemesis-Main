#include "Havok/hkpStiffSpringChainData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpStiffSpringChainData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpStiffSpringChainData::Class; });

nemesis::hkpStiffSpringChainData::hkpStiffSpringChainData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintChainData(Class)
{
}

const nemesis::hkClass* nemesis::hkpStiffSpringChainData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpStiffSpringChainData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintChainData::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteObject("atoms", m_atoms);                 // offset: 12/24 size: 12/24 align: 4/8
    serializer.WriteObject("infos", m_infos);                 // offset: 24/48 size: 12/16 align: 4/8
    serializer.WriteValue("tau", m_tau);                      // offset: 36/64 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);              // offset: 40/68 size: 4/4 align: 4/4
    serializer.WriteValue("cfm", m_cfm);                      // offset: 44/72 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());              // offset: 48/76 size: 0/4
    // class size: 48/80 align: 4/8
}

void nemesis::hkpStiffSpringChainData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintChainData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadObject("atoms", m_atoms);                      // offset: 12/24 size: 12/24 align: 4/8
    deserializer.ReadObject("infos", m_infos);                      // offset: 24/48 size: 12/16 align: 4/8
    deserializer.ReadValue("tau", m_tau);                           // offset: 36/64 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);                   // offset: 40/68 size: 4/4 align: 4/4
    deserializer.ReadValue("cfm", m_cfm);                           // offset: 44/72 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 48/76 size: 0/4
    // class size: 48/80 align: 4/8
}
