#include "Havok/hkpHingeLimitsDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpHingeLimitsDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpHingeLimitsDataAtoms::Class; });

nemesis::hkpHingeLimitsDataAtoms::hkpHingeLimitsDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpHingeLimitsDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpHingeLimitsDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("rotations", m_rotations); // offset: 0/0 size: 112/112 align: 16/16
    serializer.WriteObject("angLimit", m_angLimit);   // offset: 112/112 size: 16/16 align: 4/4
    serializer.WriteObject("2dAng", m_2dAng);         // offset: 128/128 size: 4/4 align: 2/2
    serializer.Skip(12);                              // offset: 132/132 size: 12/12
    // class size: 144/144 align: 16/16
}

void nemesis::hkpHingeLimitsDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("rotations", m_rotations); // offset: 0/0 size: 112/112 align: 16/16
    deserializer.ReadObject("angLimit", m_angLimit);   // offset: 112/112 size: 16/16 align: 4/4
    deserializer.ReadObject("2dAng", m_2dAng);         // offset: 128/128 size: 4/4 align: 2/2
    deserializer.Skip(12);                             // offset: 132/132 size: 12/12
    // class size: 144/144 align: 16/16
}
