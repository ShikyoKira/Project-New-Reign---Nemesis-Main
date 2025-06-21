#include "Havok/hkpPulleyConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPulleyConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPulleyConstraintDataAtoms::Class; });

nemesis::hkpPulleyConstraintDataAtoms::hkpPulleyConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpPulleyConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPulleyConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("translations", m_translations); // offset: 0/0 size: 48/48 align: 16/16
    serializer.WriteObject("pulley", m_pulley);             // offset: 48/48 size: 64/64 align: 16/16
    // class size: 112/112 align: 16/16
}

void nemesis::hkpPulleyConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("translations", m_translations); // offset: 0/0 size: 48/48 align: 16/16
    deserializer.ReadObject("pulley", m_pulley);             // offset: 48/48 size: 64/64 align: 16/16
    // class size: 112/112 align: 16/16
}
