#include "Havok/hkConstraintInternal.h"
#include "Havok/hkpEntity.h"

REGISTER_HAVOK_POINTER_SOURCE(hkConstraintInternal,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkConstraintInternal::Class; });

nemesis::hkConstraintInternal::hkConstraintInternal() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkConstraintInternal::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkConstraintInternal::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("constraint", m_constraint);                // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("entities", m_entities);                    // offset: 4/8 size: 8/16 align: 4/8
    serializer.WriteObject("atoms", m_atoms);                          // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("atomsSize", m_atomsSize);                   // offset: 16/32 size: 2/2 align: 2/2
    serializer.WriteValue("callbackRequest", m_callbackRequest);       // offset: 18/34 size: 1/1 align: 1/1
    serializer.WriteValue("priority", m_priority);                     // offset: 19/35 size: 1/1 align: 1/1
    serializer.WriteValue("sizeOfSchemas", m_sizeOfSchemas);           // offset: 20/36 size: 2/2 align: 2/2
    serializer.WriteValue("numSolverResults", m_numSolverResults);     // offset: 22/38 size: 2/2 align: 2/2
    serializer.WriteValue("numSolverElemTemps", m_numSolverElemTemps); // offset: 24/40 size: 2/2 align: 2/2
    serializer.WriteValue("whoIsMaster", m_whoIsMaster);               // offset: 26/42 size: 1/1 align: 1/1
    serializer.WriteValue("constraintType", m_constraintType);         // offset: 27/43 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                       // offset: 28/44 size: 0/4

    serializer.WriteObject("runtime", m_runtime);                      // offset: 28/48 size: 4/8 align: 4/8
    serializer.WriteValue("runtimeSize", m_runtimeSize);               // offset: 32/56 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("slaveIndex", m_slaveIndex); // offset: 34/58 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());                       // offset: 36/60 size: 0/4
    // class size: 36/64 align: 4/8
}

void nemesis::hkConstraintInternal::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("constraint", m_constraint);                // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("entities", m_entities);                    // offset: 4/8 size: 8/16 align: 4/8
    deserializer.ReadObject("atoms", m_atoms);                          // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("atomsSize", m_atomsSize);                   // offset: 16/32 size: 2/2 align: 2/2
    deserializer.ReadValue("callbackRequest", m_callbackRequest);       // offset: 18/34 size: 1/1 align: 1/1
    deserializer.ReadValue("priority", m_priority);                     // offset: 19/35 size: 1/1 align: 1/1
    deserializer.ReadValue("sizeOfSchemas", m_sizeOfSchemas);           // offset: 20/36 size: 2/2 align: 2/2
    deserializer.ReadValue("numSolverResults", m_numSolverResults);     // offset: 22/38 size: 2/2 align: 2/2
    deserializer.ReadValue("numSolverElemTemps", m_numSolverElemTemps); // offset: 24/40 size: 2/2 align: 2/2
    deserializer.ReadValue("whoIsMaster", m_whoIsMaster);               // offset: 26/42 size: 1/1 align: 1/1
    deserializer.ReadValue("constraintType", m_constraintType);         // offset: 27/43 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 28/44 size: 0/4

    deserializer.ReadObject("runtime", m_runtime);        // offset: 28/48 size: 4/8 align: 4/8
    deserializer.ReadValue("runtimeSize", m_runtimeSize); // offset: 32/56 size: 2/2 align: 2/2
    deserializer.ReadValue("slaveIndex", m_slaveIndex);   // offset: 34/58 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 36/60 size: 0/4
    // class size: 36/64 align: 4/8
}
