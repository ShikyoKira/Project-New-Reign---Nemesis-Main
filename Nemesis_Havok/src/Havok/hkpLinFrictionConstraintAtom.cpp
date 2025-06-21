#include "Havok/hkpLinFrictionConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpLinFrictionConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpLinFrictionConstraintAtom::Class; });

nemesis::hkpLinFrictionConstraintAtom::hkpLinFrictionConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpLinFrictionConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpLinFrictionConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer);           // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("isEnabled", m_isEnabled);               // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("frictionAxis", m_frictionAxis);         // offset: 3/3 size: 1/1 align: 1/1
    serializer.WriteValue("maxFrictionForce", m_maxFrictionForce); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkpLinFrictionConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer);      // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("isEnabled", m_isEnabled);               // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("frictionAxis", m_frictionAxis);         // offset: 3/3 size: 1/1 align: 1/1
    deserializer.ReadValue("maxFrictionForce", m_maxFrictionForce); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}
