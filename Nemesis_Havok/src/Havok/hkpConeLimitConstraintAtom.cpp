#include "Havok/hkpConeLimitConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConeLimitConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConeLimitConstraintAtom::Class; });

nemesis::hkpConeLimitConstraintAtom::hkpConeLimitConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpConeLimitConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConeLimitConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer);                   // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("isEnabled", m_isEnabled);                       // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("twistAxisInA", m_twistAxisInA);                 // offset: 3/3 size: 1/1 align: 1/1
    serializer.WriteValue("refAxisInB", m_refAxisInB);                     // offset: 4/4 size: 1/1 align: 1/1
    serializer.WriteValue("angleMeasurementMode", m_angleMeasurementMode); // offset: 5/5 size: 1/1 align: 1/1
    serializer.WriteValue("memOffsetToAngleOffset",
                          m_memOffsetToAngleOffset); // offset: 6/6 size: 1/1 align: 1/1
    serializer.Skip(1);                              // offset: 7/7 size: 1/1

    serializer.WriteValue("minAngle", m_minAngle); // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("maxAngle", m_maxAngle); // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("angularLimitsTauFactor",
                          m_angularLimitsTauFactor); // offset: 16/16 size: 4/4 align: 4/4
    // class size: 20/20 align: 4/4
}

void nemesis::hkpConeLimitConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("isEnabled", m_isEnabled);          // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("twistAxisInA", m_twistAxisInA);    // offset: 3/3 size: 1/1 align: 1/1
    deserializer.ReadValue("refAxisInB", m_refAxisInB);        // offset: 4/4 size: 1/1 align: 1/1
    deserializer.ReadValue("angleMeasurementMode",
                           m_angleMeasurementMode); // offset: 5/5 size: 1/1 align: 1/1
    deserializer.ReadValue("memOffsetToAngleOffset",
                           m_memOffsetToAngleOffset); // offset: 6/6 size: 1/1 align: 1/1
    deserializer.Skip(1);                             // offset: 7/7 size: 1/1

    deserializer.ReadValue("minAngle", m_minAngle); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAngle", m_maxAngle); // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("angularLimitsTauFactor",
                           m_angularLimitsTauFactor); // offset: 16/16 size: 4/4 align: 4/4
    // class size: 20/20 align: 4/4
}
