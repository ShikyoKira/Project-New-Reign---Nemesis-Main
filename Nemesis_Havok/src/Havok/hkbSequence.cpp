#include "Havok/hkbSequence.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSequence,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSequence::Class; });

nemesis::hkbSequence::hkbSequence() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbSequence::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSequence::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("eventSequencedData",
                           m_eventSequencedData); // offset: 44/80 size: 12/16 align: 4/8
    serializer.WriteObject("realVariableSequencedData",
                           m_realVariableSequencedData); // offset: 56/96 size: 12/16 align: 4/8
    serializer.WriteObject("boolVariableSequencedData",
                           m_boolVariableSequencedData); // offset: 68/112 size: 12/16 align: 4/8
    serializer.WriteObject("intVariableSequencedData",
                           m_intVariableSequencedData);        // offset: 80/128 size: 12/16 align: 4/8
    serializer.WriteValue("enableEventId", m_enableEventId);   // offset: 92/144 size: 4/4 align: 4/4
    serializer.WriteValue("disableEventId", m_disableEventId); // offset: 96/148 size: 4/4 align: 4/4
    serializer.WriteObject("stringData", m_stringData);        // offset: 100/152 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("variableIdMap",
                                           m_variableIdMap); // offset: 104/160 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("eventIdMap",
                                           m_eventIdMap); // offset: 108/168 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("nextSampleEvents",
                                           m_nextSampleEvents); // offset: 112/176 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("nextSampleReals",
                                           m_nextSampleReals); // offset: 124/192 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("nextSampleBools",
                                           m_nextSampleBools); // offset: 136/208 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("nextSampleInts",
                                           m_nextSampleInts);        // offset: 148/224 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("time", m_time);           // offset: 160/240 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("isEnabled", m_isEnabled); // offset: 164/244 size: 1/1 align: 1/1
    serializer.Skip(3);                                              // offset: 165/245 size: 3/3
    // class size: 168/248 align: 4/8
}

void nemesis::hkbSequence::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("eventSequencedData",
                            m_eventSequencedData); // offset: 44/80 size: 12/16 align: 4/8
    deserializer.ReadObject("realVariableSequencedData",
                            m_realVariableSequencedData); // offset: 56/96 size: 12/16 align: 4/8
    deserializer.ReadObject("boolVariableSequencedData",
                            m_boolVariableSequencedData); // offset: 68/112 size: 12/16 align: 4/8
    deserializer.ReadObject("intVariableSequencedData",
                            m_intVariableSequencedData);             // offset: 80/128 size: 12/16 align: 4/8
    deserializer.ReadValue("enableEventId", m_enableEventId);        // offset: 92/144 size: 4/4 align: 4/4
    deserializer.ReadValue("disableEventId", m_disableEventId);      // offset: 96/148 size: 4/4 align: 4/4
    deserializer.ReadObject("stringData", m_stringData);             // offset: 100/152 size: 4/8 align: 4/8
    deserializer.ReadObject("variableIdMap", m_variableIdMap);       // offset: 104/160 size: 4/8 align: 4/8
    deserializer.ReadObject("eventIdMap", m_eventIdMap);             // offset: 108/168 size: 4/8 align: 4/8
    deserializer.ReadObject("nextSampleEvents", m_nextSampleEvents); // offset: 112/176 size: 12/16 align: 4/8
    deserializer.ReadObject("nextSampleReals", m_nextSampleReals);   // offset: 124/192 size: 12/16 align: 4/8
    deserializer.ReadObject("nextSampleBools", m_nextSampleBools);   // offset: 136/208 size: 12/16 align: 4/8
    deserializer.ReadObject("nextSampleInts", m_nextSampleInts);     // offset: 148/224 size: 12/16 align: 4/8
    deserializer.ReadValue("time", m_time);                          // offset: 160/240 size: 4/4 align: 4/4
    deserializer.ReadValue("isEnabled", m_isEnabled);                // offset: 164/244 size: 1/1 align: 1/1
    deserializer.Skip(3);                                            // offset: 165/245 size: 3/3
    // class size: 168/248 align: 4/8
}
