#include "Havok/hkpPrismaticConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPrismaticConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPrismaticConstraintDataAtoms::Class; });

nemesis::hkpPrismaticConstraintDataAtoms::hkpPrismaticConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpPrismaticConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPrismaticConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16
    serializer.WriteObject("motor", m_motor);           // offset: 144/144 size: 16/24 align: 4/8
    serializer.WriteObject("friction", m_friction);     // offset: 160/168 size: 8/8 align: 4/4
    serializer.WriteObject("ang", m_ang);               // offset: 168/176 size: 4/4 align: 2/2
    serializer.WriteObject("lin0", m_lin0);             // offset: 172/180 size: 4/4 align: 2/2
    serializer.WriteObject("lin1", m_lin1);             // offset: 176/184 size: 4/4 align: 2/2
    serializer.WriteObject("linLimit", m_linLimit);     // offset: 180/188 size: 12/12 align: 4/4
    serializer.Pad(16);                                 // offset: 192/200 size: 0/8
    // class size: 192/208 align: 16/16
}

void nemesis::hkpPrismaticConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16
    deserializer.ReadObject("motor", m_motor);           // offset: 144/144 size: 16/24 align: 4/8
    deserializer.ReadObject("friction", m_friction);     // offset: 160/168 size: 8/8 align: 4/4
    deserializer.ReadObject("ang", m_ang);               // offset: 168/176 size: 4/4 align: 2/2
    deserializer.ReadObject("lin0", m_lin0);             // offset: 172/180 size: 4/4 align: 2/2
    deserializer.ReadObject("lin1", m_lin1);             // offset: 176/184 size: 4/4 align: 2/2
    deserializer.ReadObject("linLimit", m_linLimit);     // offset: 180/188 size: 12/12 align: 4/4
    deserializer.Pad(16);                                // offset: 192/200 size: 0/8
    // class size: 192/208 align: 16/16
}
