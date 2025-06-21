#include "Havok/hkbStateMachineNestedStateMachineData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineNestedStateMachineData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineNestedStateMachineData::Class; });

nemesis::hkbStateMachineNestedStateMachineData::hkbStateMachineNestedStateMachineData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbStateMachineNestedStateMachineData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineNestedStateMachineData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("nestedStateMachine", m_nestedStateMachine);        // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("eventIdMap", m_eventIdMap);        // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbStateMachineNestedStateMachineData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("nestedStateMachine", m_nestedStateMachine);        // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("eventIdMap", m_eventIdMap);        // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}
