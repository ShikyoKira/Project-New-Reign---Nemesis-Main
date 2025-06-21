#include "Havok/hkbSequenceStringData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSequenceStringData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSequenceStringData::Class; });

nemesis::hkbSequenceStringData::hkbSequenceStringData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbSequenceStringData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSequenceStringData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("eventNames", m_eventNames);       // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("variableNames", m_variableNames); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

void nemesis::hkbSequenceStringData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("eventNames", m_eventNames);        // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("variableNames", m_variableNames);  // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}
