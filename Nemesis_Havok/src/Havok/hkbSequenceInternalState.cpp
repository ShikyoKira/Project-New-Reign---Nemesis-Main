#include "Havok/hkbSequenceInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSequenceInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSequenceInternalState::Class; });

nemesis::hkbSequenceInternalState::hkbSequenceInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbSequenceInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSequenceInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);           // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("nextSampleEvents", m_nextSampleEvents); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("nextSampleReals", m_nextSampleReals);   // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("nextSampleBools", m_nextSampleBools);   // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("nextSampleInts", m_nextSampleInts);     // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteValue("time", m_time);                          // offset: 56/80 size: 4/4 align: 4/4
    serializer.WriteValue("isEnabled", m_isEnabled);                // offset: 60/84 size: 1/1 align: 1/1
    serializer.Skip(3);                                             // offset: 61/85 size: 3/3
    // class size: 64/88 align: 4/8
}

void nemesis::hkbSequenceInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);      // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("nextSampleEvents", m_nextSampleEvents); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("nextSampleReals", m_nextSampleReals);   // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("nextSampleBools", m_nextSampleBools);   // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("nextSampleInts", m_nextSampleInts);     // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadValue("time", m_time);                          // offset: 56/80 size: 4/4 align: 4/4
    deserializer.ReadValue("isEnabled", m_isEnabled);                // offset: 60/84 size: 1/1 align: 1/1
    deserializer.Skip(3);                                            // offset: 61/85 size: 3/3
    // class size: 64/88 align: 4/8
}
