#include "Havok/hkpTwistLimitConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpTwistLimitConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpTwistLimitConstraintAtom::Class; });

nemesis::hkpTwistLimitConstraintAtom::hkpTwistLimitConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpTwistLimitConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpTwistLimitConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("isEnabled", m_isEnabled);     // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("twistAxis", m_twistAxis);     // offset: 3/3 size: 1/1 align: 1/1
    serializer.WriteValue("refAxis", m_refAxis);         // offset: 4/4 size: 1/1 align: 1/1
    serializer.Skip(3);                                  // offset: 5/5 size: 3/3

    serializer.WriteValue("minAngle", m_minAngle); // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("maxAngle", m_maxAngle); // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("angularLimitsTauFactor",
                          m_angularLimitsTauFactor); // offset: 16/16 size: 4/4 align: 4/4
    // class size: 20/20 align: 4/4
}

void nemesis::hkpTwistLimitConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("isEnabled", m_isEnabled);          // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("twistAxis", m_twistAxis);          // offset: 3/3 size: 1/1 align: 1/1
    deserializer.ReadValue("refAxis", m_refAxis);              // offset: 4/4 size: 1/1 align: 1/1
    deserializer.Skip(3);                                      // offset: 5/5 size: 3/3

    deserializer.ReadValue("minAngle", m_minAngle); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAngle", m_maxAngle); // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("angularLimitsTauFactor",
                           m_angularLimitsTauFactor); // offset: 16/16 size: 4/4 align: 4/4
    // class size: 20/20 align: 4/4
}
