#include "Havok/hkpBallSocketChainData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBallSocketChainData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBallSocketChainData::Class; });

nemesis::hkpBallSocketChainData::hkpBallSocketChainData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintChainData(Class)
{
}

const nemesis::hkClass* nemesis::hkpBallSocketChainData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBallSocketChainData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintChainData::SerializeTo(serializer);      // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteObject("atoms", m_atoms);                      // offset: 12/24 size: 12/24 align: 4/8
    serializer.WriteObject("infos", m_infos);                      // offset: 24/48 size: 12/16 align: 4/8
    serializer.WriteValue("tau", m_tau);                           // offset: 36/64 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);                   // offset: 40/68 size: 4/4 align: 4/4
    serializer.WriteValue("cfm", m_cfm);                           // offset: 44/72 size: 4/4 align: 4/4
    serializer.WriteValue("maxErrorDistance", m_maxErrorDistance); // offset: 48/76 size: 4/4 align: 4/4

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("useStabilizedCode", m_useStabilizedCode);
        serializer.Pad(serializer.GetPointerSize());
    }

    // class size: 52/80 align: 4/8
}

void nemesis::hkpBallSocketChainData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintChainData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadObject("atoms", m_atoms);                      // offset: 12/24 size: 12/24 align: 4/8
    deserializer.ReadObject("infos", m_infos);                      // offset: 24/48 size: 12/16 align: 4/8
    deserializer.ReadValue("tau", m_tau);                           // offset: 36/64 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);                   // offset: 40/68 size: 4/4 align: 4/4
    deserializer.ReadValue("cfm", m_cfm);                           // offset: 44/72 size: 4/4 align: 4/4
    deserializer.ReadValue("maxErrorDistance", m_maxErrorDistance); // offset: 48/76 size: 4/4 align: 4/4

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("useStabilizedCode", m_useStabilizedCode);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    // class size: 52/80 align: 4/8
}
