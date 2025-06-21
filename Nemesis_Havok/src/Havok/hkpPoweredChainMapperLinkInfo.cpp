#include "Havok/hkpPoweredChainMapperLinkInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPoweredChainMapperLinkInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPoweredChainMapperLinkInfo::Class; });

nemesis::hkpPoweredChainMapperLinkInfo::hkpPoweredChainMapperLinkInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpPoweredChainMapperLinkInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPoweredChainMapperLinkInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("firstTargetIdx", m_firstTargetIdx);    // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("numTargets", m_numTargets);            // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteObject("limitConstraint", m_limitConstraint); // offset: 8/8 size: 4/8 align: 4/8
    // class size: 12/16 align: 4/8
}

void nemesis::hkpPoweredChainMapperLinkInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("firstTargetIdx", m_firstTargetIdx);    // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("numTargets", m_numTargets);            // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadObject("limitConstraint", m_limitConstraint); // offset: 8/8 size: 4/8 align: 4/8
    // class size: 12/16 align: 4/8
}
