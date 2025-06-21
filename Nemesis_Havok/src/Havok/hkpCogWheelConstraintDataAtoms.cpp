#include "Havok/hkpCogWheelConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCogWheelConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCogWheelConstraintDataAtoms::Class; });

nemesis::hkpCogWheelConstraintDataAtoms::hkpCogWheelConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpCogWheelConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCogWheelConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16
    serializer.WriteObject("cogWheels", m_cogWheels);   // offset: 144/144 size: 16/16 align: 4/4
    // class size: 160/160 align: 16/16
}

void nemesis::hkpCogWheelConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16
    deserializer.ReadObject("cogWheels", m_cogWheels);   // offset: 144/144 size: 16/16 align: 4/4
    // class size: 160/160 align: 16/16
}
