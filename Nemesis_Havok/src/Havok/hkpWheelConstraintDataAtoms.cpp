#include "Havok/hkpWheelConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpWheelConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpWheelConstraintDataAtoms::Class; });

nemesis::hkpWheelConstraintDataAtoms::hkpWheelConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpWheelConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpWheelConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("suspensionBase", m_suspensionBase); // offset: 0/0 size: 144/144 align: 16/16
    serializer.WriteObject("lin0Limit", m_lin0Limit);           // offset: 144/144 size: 12/12 align: 4/4
    serializer.WriteObject("lin0Soft", m_lin0Soft);             // offset: 156/156 size: 12/12 align: 4/4
    serializer.WriteObject("lin1", m_lin1);                     // offset: 168/168 size: 4/4 align: 2/2
    serializer.WriteObject("lin2", m_lin2);                     // offset: 172/172 size: 4/4 align: 2/2
    serializer.WriteObject("steeringBase", m_steeringBase);     // offset: 176/176 size: 112/112 align: 16/16
    serializer.WriteObject("2dAng", m_2dAng);                   // offset: 288/288 size: 4/4 align: 2/2
    serializer.Skip(12);                                        // offset: 292/292 size: 12/12
    // class size: 304/304 align: 16/16
}

void nemesis::hkpWheelConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("suspensionBase", m_suspensionBase); // offset: 0/0 size: 144/144 align: 16/16
    deserializer.ReadObject("lin0Limit", m_lin0Limit);           // offset: 144/144 size: 12/12 align: 4/4
    deserializer.ReadObject("lin0Soft", m_lin0Soft);             // offset: 156/156 size: 12/12 align: 4/4
    deserializer.ReadObject("lin1", m_lin1);                     // offset: 168/168 size: 4/4 align: 2/2
    deserializer.ReadObject("lin2", m_lin2);                     // offset: 172/172 size: 4/4 align: 2/2
    deserializer.ReadObject("steeringBase", m_steeringBase);     // offset: 176/176 size: 112/112 align: 16/16
    deserializer.ReadObject("2dAng", m_2dAng);                   // offset: 288/288 size: 4/4 align: 2/2
    deserializer.Skip(12);                                       // offset: 292/292 size: 12/12
    // class size: 304/304 align: 16/16
}
