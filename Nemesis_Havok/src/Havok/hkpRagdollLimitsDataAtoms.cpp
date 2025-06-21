#include "Havok/hkpRagdollLimitsDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpRagdollLimitsDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpRagdollLimitsDataAtoms::Class; });

nemesis::hkpRagdollLimitsDataAtoms::hkpRagdollLimitsDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpRagdollLimitsDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpRagdollLimitsDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("rotations", m_rotations);     // offset: 0/0 size: 112/112 align: 16/16
    serializer.WriteObject("twistLimit", m_twistLimit);   // offset: 112/112 size: 20/20 align: 4/4
    serializer.WriteObject("coneLimit", m_coneLimit);     // offset: 132/132 size: 20/20 align: 4/4
    serializer.WriteObject("planesLimit", m_planesLimit); // offset: 152/152 size: 20/20 align: 4/4
    serializer.Skip(4);                                   // offset: 172/172 size: 4/4
    // class size: 176/176 align: 16/16
}

void nemesis::hkpRagdollLimitsDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("rotations", m_rotations);     // offset: 0/0 size: 112/112 align: 16/16
    deserializer.ReadObject("twistLimit", m_twistLimit);   // offset: 112/112 size: 20/20 align: 4/4
    deserializer.ReadObject("coneLimit", m_coneLimit);     // offset: 132/132 size: 20/20 align: 4/4
    deserializer.ReadObject("planesLimit", m_planesLimit); // offset: 152/152 size: 20/20 align: 4/4
    deserializer.Skip(4);                                  // offset: 172/172 size: 4/4
    // class size: 176/176 align: 16/16
}
