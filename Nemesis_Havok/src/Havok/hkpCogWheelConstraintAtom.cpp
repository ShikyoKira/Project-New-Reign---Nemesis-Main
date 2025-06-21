#include "Havok/hkpCogWheelConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCogWheelConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCogWheelConstraintAtom::Class; });

nemesis::hkpCogWheelConstraintAtom::hkpCogWheelConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpCogWheelConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCogWheelConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(2);                                  // offset: 2/2 size: 2/2

    serializer.WriteValue("cogWheelRadiusA", m_cogWheelRadiusA); // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("cogWheelRadiusB", m_cogWheelRadiusB); // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("isScrew", m_isScrew);                 // offset: 12/12 size: 1/1 align: 1/1
    serializer.WriteValue("memOffsetToInitialAngleOffset",
                          m_memOffsetToInitialAngleOffset); // offset: 13/13 size: 1/1 align: 1/1
    serializer.WriteValue("memOffsetToPrevAngle",
                          m_memOffsetToPrevAngle); // offset: 14/14 size: 1/1 align: 1/1
    serializer.WriteValue("memOffsetToRevolutionCounter",
                          m_memOffsetToRevolutionCounter); // offset: 15/15 size: 1/1 align: 1/1
    // class size: 16/16 align: 4/4
}

void nemesis::hkpCogWheelConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(2);                                      // offset: 2/2 size: 2/2

    deserializer.ReadValue("cogWheelRadiusA", m_cogWheelRadiusA); // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("cogWheelRadiusB", m_cogWheelRadiusB); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("isScrew", m_isScrew);                 // offset: 12/12 size: 1/1 align: 1/1
    deserializer.ReadValue("memOffsetToInitialAngleOffset",
                           m_memOffsetToInitialAngleOffset); // offset: 13/13 size: 1/1 align: 1/1
    deserializer.ReadValue("memOffsetToPrevAngle",
                           m_memOffsetToPrevAngle); // offset: 14/14 size: 1/1 align: 1/1
    deserializer.ReadValue("memOffsetToRevolutionCounter",
                           m_memOffsetToRevolutionCounter); // offset: 15/15 size: 1/1 align: 1/1
    // class size: 16/16 align: 4/4
}
