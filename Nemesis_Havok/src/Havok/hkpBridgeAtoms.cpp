#include "Havok/hkpBridgeAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBridgeAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBridgeAtoms::Class; });

nemesis::hkpBridgeAtoms::hkpBridgeAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpBridgeAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBridgeAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("bridgeAtom", m_bridgeAtom); // offset: 0/0 size: 12/24 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkpBridgeAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("bridgeAtom", m_bridgeAtom); // offset: 0/0 size: 12/24 align: 4/8
    // class size: 12/24 align: 4/8
}
