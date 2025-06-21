#include "Havok/hkbBoolVariableSequencedDataSample.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBoolVariableSequencedDataSample,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBoolVariableSequencedDataSample::Class; });

nemesis::hkbBoolVariableSequencedDataSample::hkbBoolVariableSequencedDataSample() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbBoolVariableSequencedDataSample::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBoolVariableSequencedDataSample::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("time", m_time);   // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("value", m_value); // offset: 4/4 size: 1/1 align: 1/1
    serializer.Skip(3);                      // offset: 5/5 size: 3/3
    // class size: 8/8 align: 4/4
}

void nemesis::hkbBoolVariableSequencedDataSample::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("time", m_time);   // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("value", m_value); // offset: 4/4 size: 1/1 align: 1/1
    deserializer.Skip(3);                     // offset: 5/5 size: 3/3
    // class size: 8/8 align: 4/4
}
