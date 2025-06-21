#include "Havok/hkpPoweredChainMapperTarget.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPoweredChainMapperTarget,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPoweredChainMapperTarget::Class; });

nemesis::hkpPoweredChainMapperTarget::hkpPoweredChainMapperTarget() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpPoweredChainMapperTarget::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPoweredChainMapperTarget::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("chain", m_chain);        // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("infoIndex", m_infoIndex); // offset: 4/8 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());     // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkpPoweredChainMapperTarget::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("chain", m_chain);        // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("infoIndex", m_infoIndex); // offset: 4/8 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());  // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}
