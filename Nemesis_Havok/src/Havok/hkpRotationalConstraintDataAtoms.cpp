#include "Havok/hkpRotationalConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpRotationalConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpRotationalConstraintDataAtoms::Class; });

nemesis::hkpRotationalConstraintDataAtoms::hkpRotationalConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpRotationalConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpRotationalConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("rotations", m_rotations); // offset: 0/0 size: 112/112 align: 16/16
    serializer.WriteObject("ang", m_ang);             // offset: 112/112 size: 4/4 align: 2/2
    serializer.Skip(12);                              // offset: 116/116 size: 12/12
    // class size: 128/128 align: 16/16
}

void nemesis::hkpRotationalConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("rotations", m_rotations); // offset: 0/0 size: 112/112 align: 16/16
    deserializer.ReadObject("ang", m_ang);             // offset: 112/112 size: 4/4 align: 2/2
    deserializer.Skip(12);                             // offset: 116/116 size: 12/12
    // class size: 128/128 align: 16/16
}
