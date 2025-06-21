#include "Havok/hkpConstraintInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConstraintInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConstraintInfo::Class; });

nemesis::hkpConstraintInfo::hkpConstraintInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpConstraintInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstraintInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("maxSizeOfSchema", m_maxSizeOfSchema);       // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("sizeOfSchemas", m_sizeOfSchemas);           // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("numSolverResults", m_numSolverResults);     // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("numSolverElemTemps", m_numSolverElemTemps); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkpConstraintInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("maxSizeOfSchema", m_maxSizeOfSchema);       // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("sizeOfSchemas", m_sizeOfSchemas);           // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("numSolverResults", m_numSolverResults);     // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("numSolverElemTemps", m_numSolverElemTemps); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}
