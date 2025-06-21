#include "Havok/hkbExpressionData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbExpressionData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbExpressionData::Class; });

nemesis::hkbExpressionData::hkbExpressionData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbExpressionData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbExpressionData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("expression", m_expression); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("assignmentVariableIndex",
                          m_assignmentVariableIndex); // offset: 4/8 size: 4/4 align: 4/4
    serializer.WriteValue("assignmentEventIndex",
                          m_assignmentEventIndex);                       // offset: 8/12 size: 4/4 align: 4/4
    serializer.WriteValue("eventMode", m_eventMode);                     // offset: 12/16 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("raisedEvent", m_raisedEvent); // offset: 13/17 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("wasTrueInPreviousFrame",
                                          m_wasTrueInPreviousFrame); // offset: 14/18 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                     // offset: 15/19 size: 1/5
    // class size: 16/24 align: 4/8
}

void nemesis::hkbExpressionData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("expression", m_expression); // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("assignmentVariableIndex",
                           m_assignmentVariableIndex); // offset: 4/8 size: 4/4 align: 4/4
    deserializer.ReadValue("assignmentEventIndex",
                           m_assignmentEventIndex);       // offset: 8/12 size: 4/4 align: 4/4
    deserializer.ReadValue("eventMode", m_eventMode);     // offset: 12/16 size: 1/1 align: 1/1
    deserializer.ReadValue("raisedEvent", m_raisedEvent); // offset: 13/17 size: 1/1 align: 1/1
    deserializer.ReadValue("wasTrueInPreviousFrame",
                           m_wasTrueInPreviousFrame); // offset: 14/18 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());  // offset: 15/19 size: 1/5
    // class size: 16/24 align: 4/8
}
