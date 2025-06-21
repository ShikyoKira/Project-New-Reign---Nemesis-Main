#include "Havok/hkpBreakableConstraintData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBreakableConstraintData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBreakableConstraintData::Class; });

nemesis::hkpBreakableConstraintData::hkpBreakableConstraintData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpWrappedConstraintData(Class)
{
}

const nemesis::hkClass* nemesis::hkpBreakableConstraintData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBreakableConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        nemesis::hkpWrappedConstraintData::SerializeTo(serializer);
    }
    else
    {
        nemesis::hkpConstraintData::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    }

    serializer.WriteObject("atoms", m_atoms); // offset: 12/24 size: 12/24 align: 4/8

    if (cur_ver <= HavokVersion::HK_2013_2_0)
    {
        serializer.WriteObject("constraintData", m_constraintData); // offset: 24/48 size: 4/8 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2013_3_0)
    {
        serializer.WriteSerializeIgnoredValue("childRuntimeSize",
                                              m_childRuntimeSize); // offset: 28/56 size: 2/2 align: 2/2
        serializer.WriteSerializeIgnoredValue("childNumSolverResults",
                                              m_childNumSolverResults); // offset: 30/58 size: 2/2 align: 2/2
    }
    else
    {
        serializer.WriteValue("childRuntimeSize", m_childRuntimeSize); // offset: 28/56 size: 2/2 align: 2/2
        serializer.WriteValue("childNumSolverResults",
                              m_childNumSolverResults); // offset: 30/58 size: 2/2 align: 2/2
    }

    serializer.WriteValue("solverResultLimit", m_solverResultLimit); // offset: 32/60 size: 4/4 align: 4/4
    serializer.WriteValue("removeWhenBroken", m_removeWhenBroken);   // offset: 36/64 size: 1/1 align: 1/1
    serializer.WriteValue("revertBackVelocityOnBreak",
                          m_revertBackVelocityOnBreak); // offset: 37/65 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());        // offset: 38/66 size: 2/6
    // class size: 40/72 align: 4/8
}

void nemesis::hkpBreakableConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        nemesis::hkpWrappedConstraintData::DeserializeFrom(deserializer);
    }
    else
    {
        nemesis::hkpConstraintData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    }

    deserializer.ReadObject("atoms", m_atoms); // offset: 12/24 size: 12/24 align: 4/8

    if (cur_ver <= HavokVersion::HK_2013_2_0)
    {
        deserializer.ReadObject("constraintData", m_constraintData); // offset: 24/48 size: 4/8 align: 4/8
    }

    deserializer.ReadValue("childRuntimeSize", m_childRuntimeSize); // offset: 28/56 size: 2/2 align: 2/2
    deserializer.ReadValue("childNumSolverResults",
                           m_childNumSolverResults);                  // offset: 30/58 size: 2/2 align: 2/2
    deserializer.ReadValue("solverResultLimit", m_solverResultLimit); // offset: 32/60 size: 4/4 align: 4/4
    deserializer.ReadValue("removeWhenBroken", m_removeWhenBroken);   // offset: 36/64 size: 1/1 align: 1/1
    deserializer.ReadValue("revertBackVelocityOnBreak",
                           m_revertBackVelocityOnBreak); // offset: 37/65 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());     // offset: 38/66 size: 2/6
    // class size: 40/72 align: 4/8
}
