#include "Havok/hkbRealVariableSequencedDataSample.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRealVariableSequencedDataSample,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRealVariableSequencedDataSample::Class; });

nemesis::hkbRealVariableSequencedDataSample::hkbRealVariableSequencedDataSample() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbRealVariableSequencedDataSample::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRealVariableSequencedDataSample::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("time", m_time);   // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("value", m_value); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkbRealVariableSequencedDataSample::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("time", m_time);   // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("value", m_value); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}
