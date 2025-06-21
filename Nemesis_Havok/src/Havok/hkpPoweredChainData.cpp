#include "Havok/hkpPoweredChainData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPoweredChainData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPoweredChainData::Class; });

nemesis::hkpPoweredChainData::hkpPoweredChainData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintChainData(Class)
{
}

const nemesis::hkClass* nemesis::hkpPoweredChainData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPoweredChainData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintChainData::SerializeTo(serializer);      // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteObject("atoms", m_atoms);                      // offset: 12/24 size: 12/24 align: 4/8
    serializer.WriteObject("infos", m_infos);                      // offset: 24/48 size: 12/16 align: 4/8
    serializer.WriteValue("tau", m_tau);                           // offset: 36/64 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);                   // offset: 40/68 size: 4/4 align: 4/4
    serializer.WriteValue("cfmLinAdd", m_cfmLinAdd);               // offset: 44/72 size: 4/4 align: 4/4
    serializer.WriteValue("cfmLinMul", m_cfmLinMul);               // offset: 48/76 size: 4/4 align: 4/4
    serializer.WriteValue("cfmAngAdd", m_cfmAngAdd);               // offset: 52/80 size: 4/4 align: 4/4
    serializer.WriteValue("cfmAngMul", m_cfmAngMul);               // offset: 56/84 size: 4/4 align: 4/4
    serializer.WriteValue("maxErrorDistance", m_maxErrorDistance); // offset: 60/88 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                   // offset: 64/92 size: 0/4
    // class size: 64/96 align: 4/8
}

void nemesis::hkpPoweredChainData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintChainData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadObject("atoms", m_atoms);                      // offset: 12/24 size: 12/24 align: 4/8
    deserializer.ReadObject("infos", m_infos);                      // offset: 24/48 size: 12/16 align: 4/8
    deserializer.ReadValue("tau", m_tau);                           // offset: 36/64 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);                   // offset: 40/68 size: 4/4 align: 4/4
    deserializer.ReadValue("cfmLinAdd", m_cfmLinAdd);               // offset: 44/72 size: 4/4 align: 4/4
    deserializer.ReadValue("cfmLinMul", m_cfmLinMul);               // offset: 48/76 size: 4/4 align: 4/4
    deserializer.ReadValue("cfmAngAdd", m_cfmAngAdd);               // offset: 52/80 size: 4/4 align: 4/4
    deserializer.ReadValue("cfmAngMul", m_cfmAngMul);               // offset: 56/84 size: 4/4 align: 4/4
    deserializer.ReadValue("maxErrorDistance", m_maxErrorDistance); // offset: 60/88 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 64/92 size: 0/4
    // class size: 64/96 align: 4/8
}
