#include "Havok/hkbBlenderGeneratorInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBlenderGeneratorInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBlenderGeneratorInternalState::Class; });

nemesis::hkbBlenderGeneratorInternalState::hkbBlenderGeneratorInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbBlenderGeneratorInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBlenderGeneratorInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("childrenInternalStates",
                           m_childrenInternalStates);                  // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("sortedChildren", m_sortedChildren);        // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteValue("endIntervalWeight", m_endIntervalWeight);   // offset: 32/48 size: 4/4 align: 4/4
    serializer.WriteValue("numActiveChildren", m_numActiveChildren);   // offset: 36/52 size: 4/4 align: 4/4
    serializer.WriteValue("beginIntervalIndex", m_beginIntervalIndex); // offset: 40/56 size: 2/2 align: 2/2
    serializer.WriteValue("endIntervalIndex", m_endIntervalIndex);     // offset: 42/58 size: 2/2 align: 2/2
    serializer.WriteValue("initSync", m_initSync);                     // offset: 44/60 size: 1/1 align: 1/1
    serializer.WriteValue("doSubtractiveBlend", m_doSubtractiveBlend); // offset: 45/61 size: 1/1 align: 1/1
    serializer.Skip(2);                                                // offset: 46/62 size: 2/2
    // class size: 48/64 align: 4/8
}

void nemesis::hkbBlenderGeneratorInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("childrenInternalStates",
                            m_childrenInternalStates);                // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("sortedChildren", m_sortedChildren);      // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadValue("endIntervalWeight", m_endIntervalWeight); // offset: 32/48 size: 4/4 align: 4/4
    deserializer.ReadValue("numActiveChildren", m_numActiveChildren); // offset: 36/52 size: 4/4 align: 4/4
    deserializer.ReadValue("beginIntervalIndex", m_beginIntervalIndex); // offset: 40/56 size: 2/2 align: 2/2
    deserializer.ReadValue("endIntervalIndex", m_endIntervalIndex);     // offset: 42/58 size: 2/2 align: 2/2
    deserializer.ReadValue("initSync", m_initSync);                     // offset: 44/60 size: 1/1 align: 1/1
    deserializer.ReadValue("doSubtractiveBlend", m_doSubtractiveBlend); // offset: 45/61 size: 1/1 align: 1/1
    deserializer.Skip(2);                                               // offset: 46/62 size: 2/2
    // class size: 48/64 align: 4/8
}
