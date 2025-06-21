#include "Havok/hkpRackAndPinionConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpRackAndPinionConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpRackAndPinionConstraintDataAtoms::Class; });

nemesis::hkpRackAndPinionConstraintDataAtoms::hkpRackAndPinionConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpRackAndPinionConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpRackAndPinionConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("transforms", m_transforms);       // offset: 0/0 size: 144/144 align: 16/16
    serializer.WriteObject("rackAndPinion", m_rackAndPinion); // offset: 144/144 size: 12/12 align: 4/4
    serializer.Skip(4);                                       // offset: 156/156 size: 4/4
    // class size: 160/160 align: 16/16
}

void nemesis::hkpRackAndPinionConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("transforms", m_transforms);       // offset: 0/0 size: 144/144 align: 16/16
    deserializer.ReadObject("rackAndPinion", m_rackAndPinion); // offset: 144/144 size: 12/12 align: 4/4
    deserializer.Skip(4);                                      // offset: 156/156 size: 4/4
    // class size: 160/160 align: 16/16
}
