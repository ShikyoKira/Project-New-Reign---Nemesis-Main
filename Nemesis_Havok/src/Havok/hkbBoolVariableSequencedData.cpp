#include "Havok/hkbBoolVariableSequencedData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBoolVariableSequencedData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBoolVariableSequencedData::Class; });

nemesis::hkbBoolVariableSequencedData::hkbBoolVariableSequencedData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbSequencedData(Class)
{
}

const nemesis::hkClass* nemesis::hkbBoolVariableSequencedData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBoolVariableSequencedData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbSequencedData::SerializeTo(serializer);      // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("samples", m_samples);            // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("variableIndex", m_variableIndex); // offset: 20/32 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());             // offset: 24/36 size: 0/4
    // class size: 24/40 align: 4/8
}

void nemesis::hkbBoolVariableSequencedData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbSequencedData::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("samples", m_samples);            // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("variableIndex", m_variableIndex); // offset: 20/32 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());          // offset: 24/36 size: 0/4
    // class size: 24/40 align: 4/8
}
