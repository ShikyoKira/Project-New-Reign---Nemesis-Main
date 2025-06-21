#include "Havok/hkbIntVariableSequencedDataSample.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbIntVariableSequencedDataSample,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbIntVariableSequencedDataSample::Class; });

nemesis::hkbIntVariableSequencedDataSample::hkbIntVariableSequencedDataSample() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbIntVariableSequencedDataSample::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbIntVariableSequencedDataSample::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("time", m_time);   // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("value", m_value); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkbIntVariableSequencedDataSample::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("time", m_time);   // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("value", m_value); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}
