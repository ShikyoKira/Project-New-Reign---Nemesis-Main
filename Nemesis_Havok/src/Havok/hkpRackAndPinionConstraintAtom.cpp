#include "Havok/hkpRackAndPinionConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpRackAndPinionConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpRackAndPinionConstraintAtom::Class; });

nemesis::hkpRackAndPinionConstraintAtom::hkpRackAndPinionConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpRackAndPinionConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpRackAndPinionConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(2);                                  // offset: 2/2 size: 2/2

    serializer.WriteValue("pinionRadiusOrScrewPitch",
                          m_pinionRadiusOrScrewPitch); // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("isScrew", m_isScrew);       // offset: 8/8 size: 1/1 align: 1/1
    serializer.WriteValue("memOffsetToInitialAngleOffset",
                          m_memOffsetToInitialAngleOffset); // offset: 9/9 size: 1/1 align: 1/1
    serializer.WriteValue("memOffsetToPrevAngle",
                          m_memOffsetToPrevAngle); // offset: 10/10 size: 1/1 align: 1/1
    serializer.WriteValue("memOffsetToRevolutionCounter",
                          m_memOffsetToRevolutionCounter); // offset: 11/11 size: 1/1 align: 1/1
    // class size: 12/12 align: 4/4
}

void nemesis::hkpRackAndPinionConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(2);                                      // offset: 2/2 size: 2/2

    deserializer.ReadValue("pinionRadiusOrScrewPitch",
                           m_pinionRadiusOrScrewPitch); // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("isScrew", m_isScrew);       // offset: 8/8 size: 1/1 align: 1/1
    deserializer.ReadValue("memOffsetToInitialAngleOffset",
                           m_memOffsetToInitialAngleOffset); // offset: 9/9 size: 1/1 align: 1/1
    deserializer.ReadValue("memOffsetToPrevAngle",
                           m_memOffsetToPrevAngle); // offset: 10/10 size: 1/1 align: 1/1
    deserializer.ReadValue("memOffsetToRevolutionCounter",
                           m_memOffsetToRevolutionCounter); // offset: 11/11 size: 1/1 align: 1/1
    // class size: 12/12 align: 4/4
}
